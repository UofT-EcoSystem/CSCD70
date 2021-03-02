#include <llvm/CodeGen/LiveIntervals.h>
#include <llvm/CodeGen/MachineFunctionPass.h>
#include <llvm/CodeGen/RegAllocRegistry.h>
#include <llvm/CodeGen/VirtRegMap.h>
#include <llvm/InitializePasses.h>
#include <llvm/Support/raw_ostream.h>

#include <queue>

using namespace llvm;

namespace {

class RegAllocCSCD70 final : public MachineFunctionPass {
private:
  MachineFunction *MF;

  VirtRegMap *VRM;
  const TargetRegisterInfo *TRI;
  MachineRegisterInfo *MRI;
  
  // Live Intervals
  LiveIntervals *LIS;
  std::queue<LiveInterval *> LIQ;
  void enqueueLI(LiveInterval *const LI) {
    outs() << "Pushing " << *LI << " on top of the queue\n";
    LIQ.push(LI);
  }

public:
  static char ID;

  StringRef getPassName() const override { return "CSCD70 Register Allocator"; }

  RegAllocCSCD70() : MachineFunctionPass(ID) {
#define REGALLOC_CSCD70_INITIALIZE_PASS(PassName) \
    initialize##PassName##Pass(*PassRegistry::getPassRegistry())

    REGALLOC_CSCD70_INITIALIZE_PASS(VirtRegMap);
    REGALLOC_CSCD70_INITIALIZE_PASS(LiveIntervals);
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
#define REQUIRE_AND_PRESERVE_PASS(PassName)  \
    AU.addRequired<PassName>();              \
    AU.addPreserved<PassName>()

    REQUIRE_AND_PRESERVE_PASS(VirtRegMap);
    REQUIRE_AND_PRESERVE_PASS(LiveIntervals);
    MachineFunctionPass::getAnalysisUsage(AU);
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
    for (const MachineBasicBlock &MBB : MF) {
      outs() << MBB << "\n";
    }
    outs() << "\n\n";

    // The *VirtRegMap* maps virtual registers to physical registers and
    // virtual registers to stack slots.
    VRM = &getAnalysis<VirtRegMap>();
    TRI = &VRM->getTargetRegInfo();  // immutable descriptions of the target
                                     // machine register
    MRI = &VRM->getRegInfo();        // physical and virtual registers
    // freeze the reserved registers before the actual allocations begin
    MRI->freezeReservedRegs(MF);
    LIS = &getAnalysis<LiveIntervals>();

    // obtain the virtual registers and push them on top of the stack
    for (unsigned VirtualRegIdx = 0; VirtualRegIdx < MRI->getNumVirtRegs();
         ++VirtualRegIdx) {
      Register Reg = Register::index2VirtReg(VirtualRegIdx);
      // skip all registers that are defined by debug instructions
      if (MRI->reg_nodbg_empty(Reg)) {
        continue;
      }
      enqueueLI(&LIS->getInterval(Reg));
    }
    return true;
  }
};

char RegAllocCSCD70::ID = 0;

static RegisterRegAlloc X("cscd70", "CSCD70 Register Allocator",
                          []() -> FunctionPass *{
                            return new RegAllocCSCD70();
                          });

} // anonymous namespace
