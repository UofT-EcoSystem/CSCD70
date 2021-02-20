#include <llvm/Support/raw_ostream.h>

#include "Analysis.h"

void Analysis::getAnalysisUsage(AnalysisUsage &AU) const {
  // preserves all previous passes
  AU.setPreservesAll();
}

bool Analysis::runOnModule(Module &M) {
  outs() << "Analysis"
         << "\n";

  // pretend as if we have gathered some information here
  Stats.push_back(1);
  Stats.push_back(2);
  Stats.push_back(3);

  return false;
}

char Analysis::ID = 0;
static RegisterPass<Analysis> X("analysis", "Analysis");
