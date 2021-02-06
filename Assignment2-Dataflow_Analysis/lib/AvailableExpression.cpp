/**
 * @file Available Expression Dataflow Analysis
 */
#include <llvm/IR/Instructions.h>

#include "dfa/Expression.h"
#include "dfa/Framework.h"

using namespace dfa;

namespace {

class AvailableExpression final
    : public Framework<Expression, Direction::kForward>
{
 protected:
  virtual bool transferFunc(const Instruction& Inst, const BitVector & IBV,
                            BitVector& OBV) override {


    return false;
  }
  virtual void initializeDomainFromInstruction(const Instruction& Inst) override {
    if (isa<BinaryOperator>(Inst)) {
      Domain.emplace(Inst);
    }
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
