#include <llvm/CodeGen/LiveIntervals.h>
#include <llvm/CodeGen/LiveRegMatrix.h>
#include <llvm/CodeGen/MachineFunctionPass.h>
#include <llvm/CodeGen/RegAllocRegistry.h>
#include <llvm/CodeGen/RegisterClassInfo.h>
#include <llvm/CodeGen/VirtRegMap.h>
#include <llvm/InitializePasses.h>
#include <llvm/Support/raw_ostream.h>

#include <queue>

using namespace llvm;

namespace {

class RegAllocCSCD70 final : public MachineFunctionPass {
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
  std::queue<LiveInterval *> LIQ;
  void enqueueLI(LiveInterval *const LI) {
    outs() << "Pushing {Reg=" << *LI << "}\n";
    LIQ.push(LI);
  }
  LiveInterval * dequeueLI() {
    if (LIQ.empty()) { return nullptr; }
    LiveInterval *LI = LIQ.front();
    outs() << "Popping {Reg=" << *LI << "}\n";
    LIQ.pop();
    return LI;
  }
  // Live Register Matrix
  LiveRegMatrix *LRM;
  // Register Class Information
  RegisterClassInfo RCI;

  MCRegister selectOrSplit(LiveInterval *const VirtReg,
                           SmallVectorImpl<Register> *const SplitVirtRegs) {
    // 2.1. Obtain a plausible allocation order.
    ArrayRef<MCPhysReg> Order =
        RCI.getOrder(MF->getRegInfo().getRegClass(VirtReg->reg()));
    SmallVector<MCPhysReg, 16> Hints;
    bool IsHardHint =
        TRI->getRegAllocationHints(VirtReg->reg(), Order, Hints, *MF, VRM, LRM);
    if (!IsHardHint) {
      for (const MCRegister &PhysReg : Order) {
        Hints.push_back(PhysReg);
      }
    }
    outs() << "Hint Registers: [";
    for (const MCPhysReg &PhysReg : Order) {
      outs() << PhysReg << ", ";
    }
    outs() << "\n";


  }

public:
  static char ID;

  StringRef getPassName() const override { return "CSCD70 Register Allocator"; }

  RegAllocCSCD70() : MachineFunctionPass(ID) {
#define REGALLOC_CSCD70_INITIALIZE_PASS(PassName)                              \
  initialize##PassName##Pass(*PassRegistry::getPassRegistry())

    // Slot Indices
    REGALLOC_CSCD70_INITIALIZE_PASS(SlotIndexes);
    // Virtual Register Mapping
    REGALLOC_CSCD70_INITIALIZE_PASS(VirtRegMap);
    // Live Intervals
    REGALLOC_CSCD70_INITIALIZE_PASS(LiveIntervals);
    // Live Register Matrix
    REGALLOC_CSCD70_INITIALIZE_PASS(LiveRegMatrix);
  }

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
    // register classes. Note that it is NOT a pass, hence cannot be requested.
    RCI.runOnMachineFunction(MF);

    // 1. Obtain the virtual registers and push them on top of the stack.
    for (unsigned VirtualRegIdx = 0; VirtualRegIdx < MRI->getNumVirtRegs();
         ++VirtualRegIdx) {
      Register Reg = Register::index2VirtReg(VirtualRegIdx);
      // skip all unused registers
      if (MRI->reg_nodbg_empty(Reg)) {
        continue;
      }
      enqueueLI(&LIS->getInterval(Reg));
    }

    // keep traversing until the LIQ is nonempty
    while (LiveInterval *const VirtReg = dequeueLI()) {
      // again, skip all unused registers
      if (MRI->reg_nodbg_empty(VirtReg->reg())) {
        LIS->removeInterval(VirtReg->reg());
        continue;
      }

      // invalidate all previous interference queries.
      LRM->invalidateVirtRegs();

      // 2. Allocate to a physical register (if available) or split to a list of
      //    virtual registers.
      SmallVector<Register, 4> SplitVirtRegs;
      MCRegister PhysReg = selectOrSplit(VirtReg, &SplitVirtRegs);

    } // while (dequeueLI())

    return true;
  }
};

char RegAllocCSCD70::ID = 0;

static RegisterRegAlloc X("cscd70", "CSCD70 Register Allocator",
                          []() -> FunctionPass * {
                            return new RegAllocCSCD70();
                          });

} // anonymous namespace
