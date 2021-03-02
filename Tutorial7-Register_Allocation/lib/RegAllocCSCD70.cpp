#include <llvm/CodeGen/MachineFunctionPass.h>
#include <llvm/CodeGen/RegAllocRegistry.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace {

class RegAllocCSCD70 final : public MachineFunctionPass {

public:
  static char ID;

  RegAllocCSCD70() : MachineFunctionPass(ID) {}

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
#define REQUIRE_AND_PRESERVE_PASS(PassName)  \
    AU.addRequired<PassName>();              \
    AU.addPreserved<PassName>()


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
    outs() << "************************************************\n"
           << "* Machine Function\n"
           << "************************************************\n";
    for (const MachineBasicBlock &MBB : MF) {
      outs() << MBB << "\n";
    }
    outs() << "\n\n";

    LIS = getAnalysis<LiveIntervals>();

    return true;
  }
};

char RegAllocCSCD70::ID = 0;

static RegisterRegAlloc X("cscd70", "CSCD70 Register Allocator",
                          []() -> FunctionPass *{
                            return new RegAllocCSCD70();
                          });

} // anonymous namespace
