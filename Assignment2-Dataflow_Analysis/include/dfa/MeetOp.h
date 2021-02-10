#pragma once // NOLINT(llvm-header-guard)

#include <vector>

namespace dfa {

template <typename TDomainElemRepr> //
class MeetOp {
protected:
  using DomainVal_t = std::vector<TDomainElemRepr>;

public:
  virtual DomainVal_t operator()(const DomainVal_t &LHS,
                                 const DomainVal_t &RHS) const = 0;
  virtual DomainVal_t top(const size_t DomainSize) const = 0;
};

/**
 * @brief Intersection Meet Operator
 *
 * @todo(cscd70) Please complete the definition of the intersection meet
 *               operator, and modify the existing definition if necessary.
 */
class Intersect final : public MeetOp<bool> {
public:
  virtual DomainVal_t operator()(const DomainVal_t &LHS,
                                 const DomainVal_t &RHS) const override {

    return DomainVal_t();
  }
  virtual DomainVal_t top(const size_t DomainSize) const override {

    return DomainVal_t(DomainSize, false);
  }
};

} // namespace dfa
