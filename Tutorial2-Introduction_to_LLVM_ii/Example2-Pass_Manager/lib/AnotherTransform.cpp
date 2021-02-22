#include <llvm/Support/raw_ostream.h>

#include "Analysis.h"

using namespace llvm;

namespace {

class AnotherTransform final : public ModulePass {
public:
  static char ID;

  AnotherTransform() : ModulePass(ID) {}

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<Analysis>();
    AU.addPreserved<Analysis>();
  }
  virtual bool runOnModule(Module &M) override {
    outs() << "Another Transform"
           << "\n";

    std::vector<unsigned> Stats = getAnalysis<Analysis>().getStats();

    for (auto Iter = Stats.begin(); Iter != Stats.end(); ++Iter) {
      outs() << *Iter << ", ";
    }
    outs() << "\n";

    return true;
  }
};

char AnotherTransform::ID = 0;
RegisterPass<AnotherTransform> X("another-transform", "Another Transform");

} // anonymous namespace
