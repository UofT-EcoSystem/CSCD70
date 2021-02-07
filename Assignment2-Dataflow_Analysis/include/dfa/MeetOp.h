#ifndef DFA_MEETOP_H
#define DFA_MEETOP_H

#include <vector>


namespace dfa {

template<typename TDomainElemRepr>
class MeetOp {
 public:
  virtual std::vector<TDomainElemRepr> operator()(
      const std::vector<TDomainElemRepr>& LHS,
      const std::vector<TDomainElemRepr>& RHS) const = 0;
  virtual std::vector<TDomainElemRepr> top() const = 0;
};

class Intersect final : public MeetOp<bool> {
 public:
  virtual std::vector<bool> operator()(
      const std::vector<bool>& LHS, const std::vector<bool>& RHS) const override {
    return std::vector<bool>();
  }
  virtual std::vector<bool> top() const override {
    return std::vector<bool>();
  }
};

}  // namespace dfa

#endif  // DFA_MEETOP_H
