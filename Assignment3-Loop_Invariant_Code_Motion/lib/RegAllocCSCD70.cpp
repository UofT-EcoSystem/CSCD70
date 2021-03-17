/**
 * @file CSCD70 Register Allocator
 */
#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/CodeGen/LiveIntervals.h>
#include <llvm/CodeGen/LiveRangeEdit.h>
#include <llvm/CodeGen/LiveRegMatrix.h>
#include <llvm/CodeGen/LiveStacks.h>
#include <llvm/CodeGen/MachineBlockFrequencyInfo.h>
#include <llvm/CodeGen/MachineDominators.h>
#include <llvm/CodeGen/MachineFunctionPass.h>
#include <llvm/CodeGen/MachineLoopInfo.h>
#include <llvm/CodeGen/RegAllocRegistry.h>
#include <llvm/CodeGen/RegisterClassInfo.h>
#include <llvm/CodeGen/Spiller.h>
#include <llvm/CodeGen/TargetRegisterInfo.h>
#include <llvm/CodeGen/VirtRegMap.h>
#include <llvm/InitializePasses.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>

#include <cmath>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

using namespace llvm;

namespace llvm {

void initializeRACSCD70Pass(PassRegistry &Registry);

} // namespace llvm

namespace std {

template <> //
struct hash<Register> {
  size_t operator()(const Register &Reg) const {
    return DenseMapInfo<Register>::getHashValue(Reg);
  }
};

template <> //
struct greater<LiveInterval *> {
  bool operator()(LiveInterval *const &LHS, LiveInterval *const &RHS) {
    /**
     * @todo(cscd70) Please finish the implementation of this function that is
     *               used for determining whether one live interval has spill
     *               cost greater than the other.
     */
    return false;
  }
};

} // namespace std

namespace {

class RACSCD70;

class AllocationHints {
private:
  SmallVector<MCPhysReg, 16> Hints;

public:
  AllocationHints(RACSCD70 *const RA, const LiveInterval *const LI);
  SmallVectorImpl<MCPhysReg>::iterator begin() { return Hints.begin(); }
  SmallVectorImpl<MCPhysReg>::iterator end() { return Hints.end(); }
};

class RACSCD70 final : public MachineFunctionPass,
                       private LiveRangeEdit::Delegate {
private:
  MachineFunction *MF;

  SlotIndexes *SI;
  VirtRegMap *VRM;
  const TargetRegisterInfo *TRI;
  // TargetInstrInfo is used for storing registers onto the stack.
  const TargetInstrInfo *TII;
  MachineRegisterInfo *MRI;
  RegisterClassInfo RCI;
  LiveRegMatrix *LRM;
  MachineLoopInfo *MLI;
  LiveIntervals *LIS;

  /**
   * @brief Interference Graph
   */
  class IntfGraph {
  private:
    RACSCD70 *RA;

    /// Interference Relations
    std::map<LiveInterval *, std::unordered_set<Register>,
             std::greater<LiveInterval *>>
        IntfRels;

    /**
     * @brief  Try to materialize all the virtual registers (internal).
     *
     * @return (nullptr, VirtPhysRegMap) in the case when a successful
     *         materialization is made, (LI, *) in the case when unsuccessful
     *         (and LI is the live interval to spill)
     *
     * @sa tryMaterializeAll
     */
    using MaterializeResult_t =
        std::tuple<LiveInterval *,
                   std::unordered_map<LiveInterval *, MCPhysReg>>;
    MaterializeResult_t tryMaterializeAllInternal();

  public:
    explicit IntfGraph(RACSCD70 *const RA) : RA(RA) {}
    /**
     * @brief Insert a virtual register @c Reg into the interference graph.
     */
    void insert(const Register &Reg);
    /**
     * @brief Erase a virtual register @c Reg from the interference graph.
     *
     * @sa RACSCD70::LRE_CanEraseVirtReg
     */
    void erase(const Register &Reg);
    /**
     * @brief Build the whole graph.
     */
    void build();
    /**
     * @brief Try to materialize all the virtual registers.
     */
    void tryMaterializeAll();
    void clear() { IntfRels.clear(); }
  } G;

  SmallPtrSet<MachineInstr *, 32> DeadRemats;
  std::unique_ptr<Spiller> SpillerInst;

  void postOptimization() {
    SpillerInst->postOptimization();
    for (MachineInstr *const DeadInst : DeadRemats) {
      LIS->RemoveMachineInstrFromMaps(*DeadInst);
      DeadInst->eraseFromParent();
    }
    DeadRemats.clear();
    G.clear();
  }

  friend class AllocationHints;
  friend class IntfGraph;

  /// The following two methods are inherited from @c LiveRangeEdit::Delegate
  /// and implicitly used by the spiller to edit the live ranges.
  bool LRE_CanEraseVirtReg(Register Reg) override {
    /**
     * @todo(cscd70) Please implement this method.
     */
    // If the virtual register has been materialized, undo its physical
    // assignment and erase it from the interference graph.
    return true;
  }
  void LRE_WillShrinkVirtReg(Register Reg) override {
    /**
     * @todo(cscd70) Please implement this method.
     */
    // If the virtual register has been materialized, undo its physical
    // assignment and recompute its interference relationship with its
    // neighbors.
  }

  /**
   * @brief Inject stack operations to preserve caller-saved registers.
   */
  void preserveCallerSavedRegisters();

public:
  static char ID;

  StringRef getPassName() const override { return "CSCD70 Register Allocator"; }

  RACSCD70() : MachineFunctionPass(ID), G(this) {}

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    MachineFunctionPass::getAnalysisUsage(AU);
    AU.setPreservesCFG();
#define REQUIRE_AND_PRESERVE_PASS(PassName)                                    \
  AU.addRequired<PassName>();                                                  \
  AU.addPreserved<PassName>()

    REQUIRE_AND_PRESERVE_PASS(SlotIndexes);
    REQUIRE_AND_PRESERVE_PASS(VirtRegMap);
    REQUIRE_AND_PRESERVE_PASS(LiveIntervals);
    REQUIRE_AND_PRESERVE_PASS(LiveRegMatrix);
    REQUIRE_AND_PRESERVE_PASS(LiveStacks);
    REQUIRE_AND_PRESERVE_PASS(AAResultsWrapperPass);
    REQUIRE_AND_PRESERVE_PASS(MachineDominatorTree);
    REQUIRE_AND_PRESERVE_PASS(MachineLoopInfo);
    REQUIRE_AND_PRESERVE_PASS(MachineBlockFrequencyInfo);
  }

  MachineFunctionProperties getRequiredProperties() const override {
    return MachineFunctionProperties().set(
        MachineFunctionProperties::Property::NoPHIs);
  }
  MachineFunctionProperties getClearedProperties() const override {
    return MachineFunctionProperties().set(
        MachineFunctionProperties::Property::IsSSA);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;
}; // class RACSCD70

AllocationHints::AllocationHints(RACSCD70 *const RA,
                                 const LiveInterval *const LI) {
  const TargetRegisterClass *const RC = RA->MRI->getRegClass(LI->reg());

  /**
   * @todo(cscd70) Please complete this part by constructing the allocation
   *               hints, similar to the tutorial example.
   */

  outs() << "Hint Registers for Class " << RA->TRI->getRegClassName(RC)
         << ": [";
  for (const MCPhysReg &PhysReg : Hints) {
    outs() << RA->TRI->getRegAsmName(PhysReg) << ", ";
  }
  outs() << "]\n";
}

bool RACSCD70::runOnMachineFunction(MachineFunction &MF) {
  outs() << "************************************************\n"
         << "* Machine Function\n"
         << "************************************************\n";
  SI = &getAnalysis<SlotIndexes>();
  for (const MachineBasicBlock &MBB : MF) {
    MBB.print(outs(), SI);
    outs() << "\n";
  }
  outs() << "\n\n";

  this->MF = &MF;

  VRM = &getAnalysis<VirtRegMap>();
  TRI = MF.getSubtarget().getRegisterInfo();
  TII = MF.getSubtarget().getInstrInfo();
  MRI = &VRM->getRegInfo();
  MRI->freezeReservedRegs(MF);
  LIS = &getAnalysis<LiveIntervals>();
  LRM = &getAnalysis<LiveRegMatrix>();
  RCI.runOnMachineFunction(MF);
  MLI = &getAnalysis<MachineLoopInfo>();

  SpillerInst.reset(createInlineSpiller(*this, MF, *VRM));

  G.build();
  G.tryMaterializeAll();

  preserveCallerSavedRegisters();

  postOptimization();
  return true;
}

void RACSCD70::IntfGraph::insert(const Register &Reg) {
  /**
   * @todo(cscd70) Please implement this method.
   */
  // 1. Collect all VIRTUAL registers that interfere with Reg.
  // 2. Collect all PHYSICAL registers that RegUnit-interfere with Reg.
  // 3. Update the weights of Reg and all its interfering neighbors, using the
  //    formula on Lecture 6 Register Allocation Page 23.
  // 4. Insert Reg into the graph.
}

void RACSCD70::IntfGraph::erase(const Register &Reg) {
  /**
   * @todo(cscd70) Please implement this method.
   */
  // 1. ∀n ∈ neighbors(Reg), erase Reg from n's interfering set and update its
  //    weights accordingly.
  // 2. Erase Reg from the interference graph.
}

void RACSCD70::IntfGraph::build() {
  /**
   * @todo(cscd70) Please implement this method.
   */
}

RACSCD70::IntfGraph::MaterializeResult_t
RACSCD70::IntfGraph::tryMaterializeAllInternal() {
  std::unordered_map<LiveInterval *, MCPhysReg> PhysRegAssignment;

  /**
   * @todo(cscd70) Please implement this method.
   */
  // ∀r ∈ IntfRels.keys, try to materialize it. If successful, cache it in
  // PhysRegAssignment, else mark its neighbor that has the minimum spill weight
  // as to be spilled. If the virtual register has live interval across function
  // call, prioritize callee-saved registers.

  return std::make_tuple(nullptr, PhysRegAssignment);
}

void RACSCD70::IntfGraph::tryMaterializeAll() {
  std::unordered_map<LiveInterval *, MCPhysReg> PhysRegAssignment;

  /**
   * @todo(cscd70) Please implement this method.
   */
  // Keep looping until a valid assignment is made. In the case of spilling,
  // modify the interference graph accordingly.

  for (auto &PhysRegAssignPair : PhysRegAssignment) {
    RA->LRM->assign(*PhysRegAssignPair.first, PhysRegAssignPair.second);
  }
}

void RACSCD70::preserveCallerSavedRegisters() {
  /**
   * @todo(cscd70) Please implement this method.
   */
}

char RACSCD70::ID = 0;

static RegisterRegAlloc X("cscd70", "CSCD70 Register Allocator",
                          []() -> FunctionPass * { return new RACSCD70(); });

} // anonymous namespace

INITIALIZE_PASS_BEGIN(RACSCD70, "regallocscd70", "CSCD70 Register Allocator",
                      false, false)
INITIALIZE_PASS_DEPENDENCY(SlotIndexes)
INITIALIZE_PASS_DEPENDENCY(VirtRegMap)
INITIALIZE_PASS_DEPENDENCY(LiveIntervals)
INITIALIZE_PASS_DEPENDENCY(LiveRegMatrix)
INITIALIZE_PASS_DEPENDENCY(LiveStacks);
INITIALIZE_PASS_DEPENDENCY(AAResultsWrapperPass);
INITIALIZE_PASS_DEPENDENCY(MachineDominatorTree);
INITIALIZE_PASS_DEPENDENCY(MachineLoopInfo);
INITIALIZE_PASS_DEPENDENCY(MachineBlockFrequencyInfo);
INITIALIZE_PASS_END(RACSCD70, "regallocscd70", "CSCD70 Register Allocator",
                    false, false)
