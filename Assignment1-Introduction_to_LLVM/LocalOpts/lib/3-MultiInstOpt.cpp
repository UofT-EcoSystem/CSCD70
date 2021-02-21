#include <llvm/IR/Module.h>
#include <llvm/Pass.h>

using namespace llvm;

namespace {

class MultiInstOpt final : public FunctionPass {
public:
  static char ID;

  MultiInstOpt() : FunctionPass(ID) {}

  /**
   * @todo(cscd70) Please complete the methods below.
   */
  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {}

  virtual bool runOnFunction(Function &F) override { return false; }
}; // class MultiInstOpt

char MultiInstOpt::ID = 0;
RegisterPass<MultiInstOpt> X("multi-inst-opt",
                             "CSCD70: Multi-Instruction Optimization");

} // anonymous namespace
