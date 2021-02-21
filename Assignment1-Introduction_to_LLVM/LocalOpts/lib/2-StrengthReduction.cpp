#include <llvm/IR/Module.h>
#include <llvm/Pass.h>

using namespace llvm;

namespace {

class StrengthReduction final : public FunctionPass {
public:
  static char ID;

  StrengthReduction() : FunctionPass(ID) {}

  /**
   * @todo(cscd70) Please complete the methods below.
   */
  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {}

  virtual bool runOnFunction(Function &F) override { return false; }
}; // class StrengthReduction

char StrengthReduction::ID = 0;
RegisterPass<StrengthReduction> X("strength-reduction",
                                  "CSCD70: Strength Reduction");

} // anonymous namespace
