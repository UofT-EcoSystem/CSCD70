#pragma once // NOLINT(llvm-header-guard)

#include <functional>

#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace {

/**
 * @brief A wrapper for binary expressions.
 */
struct Expression {
  const unsigned Opcode;
  const Value *const LHS = nullptr, *const RHS = nullptr;
  Expression(const BinaryOperator &BinaryOp)
      : Opcode(BinaryOp.getOpcode()), LHS(BinaryOp.getOperand(0)),
        RHS(BinaryOp.getOperand(1)) {}
  /**
   * @todo(cscd70) Please complete the comparator.
   */
  bool operator==(const Expression &Expr) const { return false; }
};

inline raw_ostream &operator<<(raw_ostream &Outs, const Expression &Expr) {
  Outs << "[" << Instruction::getOpcodeName(Expr.Opcode) << " ";
  Expr.LHS->printAsOperand(Outs, false);
  Outs << ", ";
  Expr.RHS->printAsOperand(Outs, false);
  Outs << "]";
  return Outs;
}

} // anonymous namespace

namespace std {

/**
 * @brief  Construct a hash code for @c Expression .
 */
template <> struct hash<Expression> {
  size_t operator()(const Expression &Expr) const {
    hash<unsigned> UnsignedHasher;
    hash<const Value *> PValueHasher;
    size_t OpcodeHashVal = UnsignedHasher(Expr.Opcode);
    size_t LHSOperandHashVal = PValueHasher(Expr.LHS);
    size_t RHSOperandHashVal = PValueHasher(Expr.RHS);
    return OpcodeHashVal ^ (LHSOperandHashVal << 1) ^ (RHSOperandHashVal << 1);
  }
};

} // namespace std
