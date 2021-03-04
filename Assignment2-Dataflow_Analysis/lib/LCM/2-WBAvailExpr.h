#pragma once // NOLINT(llvm-header-guard)

/**
 * @file Will-be-Available Expression Dataflow Analysis
 */
#include "1-AntiExpr.h"

class WBAvailExprWrapperPass;

/**
 * @todo(cscd70) Provide an implementation for the will-be-available
 *               expressions, by inheriting from the DFA framework.
 */
class WBAvailExprImpl {
private:
  friend class WBAvailExprWrapperPass;
};

class WBAvailExprWrapperPass : public FunctionPass {
private:
  /**
   * @todo(cscd70) Uncomment the following line after the completion of
   *               @c WBAvailExprImpl .
   */
  // WBAvailExprImpl WBAvailExpr;

public:
  static char ID;

  WBAvailExprWrapperPass() : FunctionPass(ID) {}

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    /**
     * @todo(cscd70) Request that the anticipated expressions to run first.
     */
    AU.setPreservesAll();
  }
  bool runOnFunction(Function &F) override {
    errs() << "* Will-Be-Available Expression *"
           << "\n";
    /**
     * @todo(cscd70) Obtain the domain and analysis results from the anticipated
     *               expressions, and then invoke the method @c runOnFunction
     *               from @c WBAvailExprImpl .
     */
    return false;
  }

  /**
   * @todo(cscd70) Obtain the @c InstDomainValMap and boundary values at each BB
   *               from @c WBAvailExprImpl .
   */
  std::unordered_map<const Instruction *, std::vector<bool>>
  getInstDomainValMap() const {
    return std::unordered_map<const Instruction *, std::vector<bool>>();
  }
  std::unordered_map<const BasicBlock *, std::vector<bool>>
  getBoundaryVals() const {
    return std::unordered_map<const BasicBlock *, std::vector<bool>>();
  }
};
