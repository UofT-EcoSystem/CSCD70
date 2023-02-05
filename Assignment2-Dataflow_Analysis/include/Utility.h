#pragma once // NOLINT(llvm-header-guard)

#include <llvm/Support/raw_ostream.h>

#include <cstdlib>
#include <functional>

inline void hashCombine(std::size_t *const Seed) {}

/// @brief This is a function for combining hash values from multiple members.
///        Example usage:
///
///            size_t seed;
///            hashCombine(&seed, memberA, memberB);
///
///        Reference: https://stackoverflow.com/a/2595226/6320608
template <typename T, typename... TVarArg>
inline void hashCombine(std::size_t *const Seed, const T &Item,
                        TVarArg... VarArg) {
  std::hash<T> Hasher;
  (*Seed) ^= Hasher(Item) + 0x9e3779b9 + ((*Seed) << 6) + ((*Seed) >> 2);
  hashCombine(Seed, VarArg...);
}

class InternalRuntimeChecker {
private:
  const bool Cond;

public:
  InternalRuntimeChecker(const bool Cond) : Cond(Cond) {}
  ~InternalRuntimeChecker() {
    if (!Cond) {
      llvm::errs() << "\n";
      abort();
    }
  }
  operator bool() const { return !Cond; }
};

struct InternalInfoLogger {
  llvm::raw_ostream &Outs;
  InternalInfoLogger(llvm::raw_ostream &Outs = llvm::outs()) : Outs(Outs) {}
  ~InternalInfoLogger() { Outs << "\n"; }
  operator bool() const { return true; }
};

#define CHECK(cond)                                                            \
  if (auto Checker = InternalRuntimeChecker((cond)))                           \
  llvm::errs() << "[" << __FILE__ << ":" << __LINE__ << ", E] "

#define LOG_INFO                                                               \
  if (auto Logger = InternalInfoLogger())                                      \
  llvm::outs() << "[" << __FILE__ << ":" << __LINE__ << ", I] "

#define LOG_ANALYSIS_INFO                                                      \
  if (auto Logger = InternalInfoLogger(llvm::errs()))                          \
  llvm::errs() << "[" << getName() << "] "

template <typename T>
inline bool operator!=(const std::vector<T> &LHS, const std::vector<T> &RHS) {
  CHECK(LHS.size() == RHS.size()) << "Size of domain values has to be the same";
  for (size_t Idx = 0; Idx < LHS.size(); ++Idx) {
    if (LHS[Idx] != RHS[Idx]) {
      return true;
    }
  }
  return false;
}
