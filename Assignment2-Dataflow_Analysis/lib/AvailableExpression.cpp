/**
 * @file Available Expression Dataflow Analysis
 */
#include <llvm/IR/Instructions.h>
#include <llvm/Pass.h>

#include "dfa/Expression.h"
#include "dfa/Framework.h"

using namespace dfa;
using namespace llvm;


namespace {

using AvailExprFrameworkBase
    = Framework<Expression, bool, Direction::kForward, Intersect>;

class AvailableExpression final
    : public AvailExprFrameworkBase,
      public FunctionPass
{
 protected:
  virtual bool transferFunc(const Instruction& Inst,
                            const std::vector<bool>& IBV,
                            std::vector<bool>& OBV) override {


    return false;
  }
public:
  static char ID;

  AvailableExpression() : AvailExprFrameworkBase(), FunctionPass(ID) {}
  virtual ~AvailableExpression() override {}

  bool runOnFunction(Function& F) override {
    return AvailExprFrameworkBase::runOnFunction(F);
  }
};

char AvailableExpression::ID = 1; 
RegisterPass<AvailableExpression> Y(
    "avail-expr",
    "Available Expression");

}  // anonymous namespace
