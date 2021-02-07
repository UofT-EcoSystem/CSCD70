/**
 * @file Available Expression Dataflow Analysis
 */
#include <llvm/IR/Instructions.h>
#include <llvm/Pass.h>

#include "Expression.h"
#include "dfa/Framework.h"

using namespace dfa;
using namespace llvm;


namespace {

using AvailExprFrameworkBase
    = Framework<Expression, bool, Direction::kForward, Intersect>;

class AvailExpr final
    : public AvailExprFrameworkBase,
      public FunctionPass
{
 protected:
  virtual bool transferFunc(const Instruction& Inst,
                            const std::vector<bool>& IBV,
                            std::vector<bool>& OBV) override {
    /**
     * @todo(cscd70) Please complete the definition of the transfer function.
     */

    return false;
  }
public:
  static char ID;

  AvailExpr() : AvailExprFrameworkBase(), FunctionPass(ID) {}
  virtual ~AvailExpr() override {}

  bool runOnFunction(Function& F) override {
    return AvailExprFrameworkBase::runOnFunction(F);
  }
};

char AvailExpr::ID = 1; 
RegisterPass<AvailExpr> Y(
    "avail-expr",
    "Available Expression");

}  // anonymous namespace
