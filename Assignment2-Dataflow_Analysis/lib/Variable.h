#pragma once // NOLINT(llvm-header-guard)

#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>

using namespace dfa;
using namespace llvm;

/**
 * @brief A wrapper for variable.
 */
struct Variable {
  const Value *const V = nullptr;
  Variable(const Value *const Var) : V(Var) {}
  bool operator==(const Variable &Var) const { return V == Var.V; }
};

inline raw_ostream &operator<<(raw_ostream &Outs, const Variable &Var) {
  Var.V->printAsOperand(Outs, false);
  return Outs;
}
