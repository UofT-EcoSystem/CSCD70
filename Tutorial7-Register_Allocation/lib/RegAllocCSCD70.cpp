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

namespace llvm {

void initializeRACSCD70Pass(PassRegistry &Registry);

} // namespace llvm

namespace {

class RACSCD70 final : public MachineFunctionPass {
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
  LiveInterval *dequeueLI() {
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
    return 0;
  }

public:
  static char ID;

  StringRef getPassName() const override { return "CSCD70 Register Allocator"; }

  RACSCD70() : MachineFunctionPass(ID) {}

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
    while (LiveInterval *const LI = dequeueLI()) {
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

    } // while (dequeueLI())

    return true;
  }
};

char RACSCD70::ID = 0;

static RegisterRegAlloc X("cscd70", "CSCD70 Register Allocator",
                          []() -> FunctionPass * { return new RACSCD70(); });

} // anonymous namespace

INITIALIZE_PASS_BEGIN(RACSCD70, "regallocscd70", "CSCD70 Register Allocator",
                      false, false)
// Slot Indices
INITIALIZE_PASS_DEPENDENCY(SlotIndexes)
// Virtual Register Mapping
INITIALIZE_PASS_DEPENDENCY(VirtRegMap)
// Live Intervals
INITIALIZE_PASS_DEPENDENCY(LiveIntervals)
// Live Register Matrix
INITIALIZE_PASS_DEPENDENCY(LiveRegMatrix)
INITIALIZE_PASS_END(RACSCD70, "regallocscd70", "CSCD70 Register Allocator",
                    false, false)
