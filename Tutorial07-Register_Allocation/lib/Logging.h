#pragma once // NOLINT(llvm-header-guard)

#include <llvm/Support/raw_ostream.h>

namespace internal {

struct InfoLogger {
  llvm::raw_ostream &Outs;
  InfoLogger(llvm::raw_ostream &Outs = llvm::outs()) : Outs(Outs) {}
  ~InfoLogger() { Outs << "\n"; }
  operator bool() const { return true; }
};

} // namespace internal

#define LOG_INFO                                                               \
  if (auto Logger = internal::InfoLogger())                                    \
  llvm::outs() << "[" << __FILE__ << ":" << __LINE__ << ", I] "

#define BOLD(str) "\033[1m" << str << "\033[0m"
