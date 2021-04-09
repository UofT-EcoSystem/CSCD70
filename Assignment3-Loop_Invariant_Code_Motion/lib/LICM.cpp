/**
 * @file Loop Invariant Code Motion
 */
#include <llvm/Analysis/LoopPass.h>
#include <llvm/Analysis/ValueTracking.h>

using namespace llvm;

namespace {

class LoopInvariantCodeMotion final : public LoopPass {
public:
  static char ID;

  LoopInvariantCodeMotion() : LoopPass(ID) {}

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    /**
     * @todo(cscd70) Request the dominator tree and the loop simplify pass.
     */
    AU.setPreservesCFG();
  }

  /**
   * @todo(cscd70) Please finish the implementation of this method.
   */
  virtual bool runOnLoop(Loop *L, LPPassManager &LPM) override { return false; }
};

char LoopInvariantCodeMotion::ID = 0;
RegisterPass<LoopInvariantCodeMotion> X("loop-invariant-code-motion",
                                        "Loop Invariant Code Motion");

} // anonymous namespace
