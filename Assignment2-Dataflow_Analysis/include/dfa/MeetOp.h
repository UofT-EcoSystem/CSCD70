#pragma once // NOLINT(llvm-header-guard)

#include <vector>

namespace dfa {

template <typename TDomainElemRepr> //
class MeetOp {
public:
  virtual std::vector<TDomainElemRepr>
  operator()(const std::vector<TDomainElemRepr> &LHS,
             const std::vector<TDomainElemRepr> &RHS) const = 0;
  virtual std::vector<TDomainElemRepr> top(const size_t DomainSize) const = 0;
};

/**
 * @brief Intersection Meet Operator
 *
 * @todo(cscd70) Please complete the definition of the intersection meet
 *               operator.
 */
class Intersect final : public MeetOp<bool> {
public:
  virtual std::vector<bool>
  operator()(const std::vector<bool> &LHS,
             const std::vector<bool> &RHS) const override {

    return std::vector<bool>();
  }
  virtual std::vector<bool> top(const size_t DomainSize) const override {

    return std::vector<bool>(DomainSize, false);
  }
};

} // namespace dfa
