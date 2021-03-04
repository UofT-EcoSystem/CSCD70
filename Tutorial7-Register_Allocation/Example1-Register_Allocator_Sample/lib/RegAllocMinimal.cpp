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
#include <llvm/CodeGen/VirtRegMap.h>
#include <llvm/InitializePasses.h>
#include <llvm/Support/raw_ostream.h>

#include <queue>

using namespace llvm;

namespace llvm {

void initializeRAMinimalPass(PassRegistry &Registry);

} // namespace llvm

namespace {

class RAMinimal final : public MachineFunctionPass,
                        private LiveRangeEdit::Delegate {
private:
  MachineFunction *MF;

  // Slot Indices
  const SlotIndexes *SI;
  // Virtual Register Mapping
  VirtRegMap *VRM;
  const TargetRegisterInfo *TRI;
  MachineRegisterInfo *MRI;
  // Live Intervals
  LiveIntervals *LIS;
  std::queue<LiveInterval *> LIQ; // FIFO Queue
  void enqueue(LiveInterval *const LI) {
    outs() << "Pushing {Reg=" << *LI << "}\n";
    LIQ.push(LI);
  }
  LiveInterval *dequeue() {
    if (LIQ.empty()) {
      return nullptr;
    }
    LiveInterval *LI = LIQ.front();
    outs() << "Popping {Reg=" << *LI << "}\n";
    LIQ.pop();
    return LI;
  }
  // Live Register Matrix
  LiveRegMatrix *LRM;
  // Register Class Information
  RegisterClassInfo RCI;

  // Spiller
  std::unique_ptr<Spiller> SpillerInst;
  SmallPtrSet<MachineInstr *, 32> DeadRemats;

  MCRegister selectOrSplit(LiveInterval *const LI,
                           SmallVectorImpl<Register> *const SplitVirtRegs) {
    // 2.1. Obtain a plausible allocation order.
    ArrayRef<MCPhysReg> Order =
        RCI.getOrder(MF->getRegInfo().getRegClass(LI->reg()));
    SmallVector<MCPhysReg, 16> Hints;
    bool IsHardHint =
        TRI->getRegAllocationHints(LI->reg(), Order, Hints, *MF, VRM, LRM);
    if (!IsHardHint) {
      for (const MCPhysReg &PhysReg : Order) {
        Hints.push_back(PhysReg);
      }
    }
    outs() << "Hint Registers: [";
    for (const MCPhysReg &PhysReg : Order) {
      outs() << TRI->getRegAsmName(PhysReg) << ", ";
    }
    outs() << "]\n";

    for (MCRegister PhysReg : Hints) {
      // 2.2. Check for interference on physical registers.
      switch (LRM->checkInterference(*LI, PhysReg)) {
      case LiveRegMatrix::IK_Free:
        // Here we directly (and naively) return the first physical register
        // that is available.
        outs() << "Allocating physical register " << TRI->getRegAsmName(PhysReg)
               << "\n";
        return PhysReg;
      case LiveRegMatrix::IK_VirtReg:
        continue;
      default:
        continue;
      }
    }
    // Inform the caller that the virtual register has been spilled.
    LiveRangeEdit LRE(LI, *SplitVirtRegs, *MF, *LIS, VRM, this, &DeadRemats);
    SpillerInst->spill(LRE);
    return 0;
  }

public:
  static char ID;

  StringRef getPassName() const override {
    return "Minimal Register Allocator";
  }

  RAMinimal() : MachineFunctionPass(ID) {}

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

    // implicitly requested by the spiller
    REQUIRE_AND_PRESERVE_PASS(LiveStacks);
    REQUIRE_AND_PRESERVE_PASS(AAResultsWrapperPass);
    REQUIRE_AND_PRESERVE_PASS(MachineDominatorTree);
    REQUIRE_AND_PRESERVE_PASS(MachineLoopInfo);
    REQUIRE_AND_PRESERVE_PASS(MachineBlockFrequencyInfo);
  }

  // Request that PHINode's are removed before doing the register allocation.
  MachineFunctionProperties getRequiredProperties() const override {
    return MachineFunctionProperties().set(
        MachineFunctionProperties::Property::NoPHIs);
  }
  MachineFunctionProperties getClearedProperties() const override {
    return MachineFunctionProperties().set(
        MachineFunctionProperties::Property::IsSSA);
  }

  bool runOnMachineFunction(MachineFunction &MF) override {
    this->MF = &MF;

    outs() << "************************************************\n"
           << "* Machine Function\n"
           << "************************************************\n";
    // THe *SlotIndexes* maps each machine instruction to a unique ID.
    SI = &getAnalysis<SlotIndexes>();
    for (const MachineBasicBlock &MBB : MF) {
      MBB.print(outs(), SI);
      outs() << "\n";
    }
    outs() << "\n\n";

    // 0. Get the requested analysis requests from the following passes:
    //    - VirtRegMap
    //    - LiveIntervals
    //    - LiveRegMatrix
    //    and setup the spiller.

    // The *VirtRegMap* maps virtual registers to physical registers and
    // virtual registers to stack slots.
    VRM = &getAnalysis<VirtRegMap>();
    TRI = &VRM->getTargetRegInfo(); // immutable descriptions of the target
                                    // machine register
    MRI = &VRM->getRegInfo();       // physical and virtual registers
    // freeze the reserved registers before the actual allocations begin
    MRI->freezeReservedRegs(MF);
    // The *LiveIntervals* describe the live range of each virtual register.
    LIS = &getAnalysis<LiveIntervals>();
    // The *LiveRegMatrix* keeps track of virtual register interference along
    // two dimensions: slot indices and register units. The matrix is used by
    // register allocators to ensure that no interfering virtual registers get
    // assigned to overlapping physical registers.
    LRM = &getAnalysis<LiveRegMatrix>();
    // The *RegisterClassInfo* provides dynamic information about target
    // register classes. We will be leveraging it to obtain a plausible
    // allocation order of physical registers.
    RCI.runOnMachineFunction(MF);

    // The *Spiller* is, of course, responsible for spilling.
    SpillerInst.reset(createInlineSpiller(*this, MF, *VRM));

    // 1. Obtain the virtual registers and push them on top of the stack.
    for (unsigned VirtualRegIdx = 0; VirtualRegIdx < MRI->getNumVirtRegs();
         ++VirtualRegIdx) {
      Register Reg = Register::index2VirtReg(VirtualRegIdx);
      // skip all unused registers
      if (MRI->reg_nodbg_empty(Reg)) {
        continue;
      }
      enqueue(&LIS->getInterval(Reg));
    }

    // keep traversing until the LIQ is nonempty
    while (LiveInterval *const LI = dequeue()) {
      // again, skip all unused registers
      if (MRI->reg_nodbg_empty(LI->reg())) {
        LIS->removeInterval(LI->reg());
        continue;
      }
      // invalidate all previous interference queries.
      LRM->invalidateVirtRegs();

      // 2. Allocate to a physical register (if available) or split to a list of
      //    virtual registers.
      SmallVector<Register, 4> SplitVirtRegs;
      MCRegister PhysReg = selectOrSplit(LI, &SplitVirtRegs);

      if (PhysReg) {
        LRM->assign(*LI, PhysReg);
      }
      // enqueue the splitted live ranges
      for (Register Reg : SplitVirtRegs) {
        LiveInterval *LI = &LIS->getInterval(Reg);
        if (MRI->reg_nodbg_empty(LI->reg())) {
          LIS->removeInterval(LI->reg());
          continue;
        }
        enqueue(LI);
      }
    } // while (dequeue())

    // cleanup
    SpillerInst->postOptimization();

    return true;
  }
};

char RAMinimal::ID = 0;

static RegisterRegAlloc X("minimal", "Minimal Register Allocator",
                          []() -> FunctionPass * { return new RAMinimal(); });

} // anonymous namespace

INITIALIZE_PASS_BEGIN(RAMinimal, "regallominimal", "Minimal Register Allocator",
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
INITIALIZE_PASS_END(RAMinimal, "regallominimal", "Minimal Register Allocator",
                    false, false)
