#pragma once // NOLINT(llvm-header-guard)

/**
 * @file Earliest Placement
 */
#include "2-WBAvailExpr.h"

class EPlaceWrapperPass;

/**
 * @todo(cscd70) Provide an implementation for the earliest placement, by
 *               inheriting from the DFA framework.
 */
class EPlaceImpl {
private:
  friend class EPlaceWrapperPass;
};

class EPlaceWrapperPass : public FunctionPass {
private:
  /**
   * @todo(cscd70) Uncomment the following line after the completion of
   *               @c EPlaceImpl .
   */
  // EPlaceImpl EPlace;

public:
  static char ID;

  EPlaceWrapperPass() : FunctionPass(ID) {}

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    /**
     * @todo(cscd70) Request that the anticipated and will-be-available
     *               expressions to run first.
     */
    AU.setPreservesAll();
  }
  bool runOnFunction(Function &F) override {
    errs() << "* Earliest Placement *"
           << "\n";
    /**
     * @todo(cscd70) Obtain the domain and analysis results from the anticipated
     *               and will-be-available expressions, and then invoke the
     *               method @c runOnFunction from @c EPlaceImpl .
     */
    return false;
  }
};
