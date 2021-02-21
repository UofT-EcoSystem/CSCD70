#pragma once // NOLINT(llvm-header-guard)

/**
 * @file Anticipated Expression Dataflow Analysis
 */
#include <llvm/Pass.h>
#include <llvm/Transforms/Utils.h>

#include <dfa/Framework.h>
#include <dfa/MeetOp.h>

#include "../Expression.h"

using namespace dfa;

class AntiExprWrapperPass;

/**
 * @todo(cscd70) Provide an implementation for the anticipated expressions, by
 *               inheriting from the DFA framework.
 */
class AntiExprImpl {
private:
  friend class AntiExprWrapperPass;
};

class AntiExprWrapperPass : public FunctionPass {
private:
  /**
   * @todo(cscd70) Uncomment the following line after the completion of
   *               @c AntiExprImpl .
   */
  // AntiExprImpl AntiExpr;

public:
  static char ID;

  AntiExprWrapperPass() : FunctionPass(ID) {}

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    /**
     * @todo(cscd70) Request that the LLVM built-in pass @c BreakCriticalEdges
     *               to run first, by leveraging its ID.
     */
    AU.setPreservesAll();
  }

  bool runOnFunction(Function &F) override {
    errs() << "* Anticipated Expression *"
           << "\n";

    /**
     * @todo(cscd70) Invoke the method @c runOnFunction from @c AntiExprImpl .
     */
    return false;
  }

  /**
   * @todo(cscd70) Obtain the @c Domain and @c InstDomainValMap from
   *               @c AntiExprImpl .
   */
  std::vector<Expression> getDomain() const {
    return std::vector<Expression>();
  }
  std::unordered_map<const Instruction *, std::vector<bool>>
  getInstDomainValMap() const {
    return std::unordered_map<const Instruction *, std::vector<bool>>();
  }
};
