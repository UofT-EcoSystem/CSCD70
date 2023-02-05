#pragma once // NOLINT(llvm-header-guard)

#include <unordered_map>
#include <vector>

namespace dfa {

template <typename TDerivedDomainElem> struct DomainBase {
  virtual bool operator==(const TDerivedDomainElem &Other) const = 0;

  /// @brief Check whether the domain element contains the given value.
  /// @param Val
  /// @return
  virtual bool contain(const llvm::Value *const Val) const = 0;
  /// @brief Replace all the occurrences of the source value with the
  ///        destination value.
  /// @param SrcVal
  /// @param DstVal
  /// @return
  virtual TDerivedDomainElem
  replaceValueWith(const llvm::Value *const SrcVal,
                   const llvm::Value *const DstVal) const = 0;

  using DomainIdMap_t = std::unordered_map<TDerivedDomainElem, size_t>;
  using DomainVector_t = std::vector<TDerivedDomainElem>;
};

} // namespace dfa
