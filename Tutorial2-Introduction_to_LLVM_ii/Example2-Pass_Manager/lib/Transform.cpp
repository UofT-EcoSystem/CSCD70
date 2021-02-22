#include <llvm/Support/raw_ostream.h>

#include "Analysis.h"

using namespace llvm;

namespace {

class Transform final : public ModulePass {
public:
  static char ID;

  Transform() : ModulePass(ID) {}

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    // If you comment this line out, the 'Analysis' pass will not be run. LLVM
    // will also give you a runtime error upon executing
    //
    //     std::vector<unsigned> stats = getAnalysis<Analysis>().getStats();
    //
    // (shown below in the 'runOnModule' method) as you have not requested pass
    // 'Analysis'.
    AU.addRequired<Analysis>();
    // If you comment this line out, the 'Analysis' pass will be run once again
    // on 'AnotherTransform'.
    AU.addPreserved<Analysis>();
  }
  virtual bool runOnModule(Module &M) override {
    outs() << "Transform"
           << "\n";

    std::vector<unsigned> Stats = getAnalysis<Analysis>().getStats();

    for (auto Iter = Stats.begin(); Iter != Stats.end(); ++Iter) {
      outs() << *Iter << ", ";
    }
    outs() << "\n";

    return false;
  }
};

char Transform::ID = 0;
RegisterPass<Transform> X("transform", "Transform");

} // anonymous namespace
