#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

#include "dfa/Framework.h"

using namespace llvm;

namespace {

/**
 * @todo(cscd70) Implement @c Liveness using the @c dfa::Framework interface.
 */
class Liveness final : public FunctionPass {
public:
  static char ID;

  Liveness() : FunctionPass(ID) {}
  virtual ~Liveness() override {}

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  virtual bool runOnFunction(Function &F) override {
    outs() << "**************************************************"
           << "\n"
           << "* Instruction-Domain Mapping"
           << "\n"
           << "**************************************************"
           << "\n";

    return false;
  }
};

char Liveness::ID = 2;
RegisterPass<Liveness> X("liveness", "Liveness");

} // anonymous namespace
