/**
 * @file Available Expression Dataflow Analysis
 */
#include <llvm/IR/Instructions.h>

#include "dfa/BC.h"
#include "dfa/Expression.h"
#include "dfa/Framework.h"
#include "dfa/MeetOp.h"

using namespace dfa;


namespace {

class AvailableExpression final
    : public Framework<Expression, Direction::kForward, Intersect>
{
 protected:
  virtual bool transferFunc(const Instruction& Inst, const BitVector & IBV,
                            BitVector& OBV) override {


    return false;
  }
public:
  static char ID;

  AvailableExpression() : dfa::Framework<>(ID) {}
  virtual ~AvailableExpression() override {}
};

char AvailableExpression::ID = 1; 
RegisterPass<AvailableExpression> Y(
    "avail-expr",
    "Available Expression");

} // namespace anonymous
