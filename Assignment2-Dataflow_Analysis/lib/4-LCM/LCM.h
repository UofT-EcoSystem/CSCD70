#pragma once // NOLINT(llvm-header-guard)

#include <llvm/IR/Function.h>
#include <llvm/IR/PassManager.h>

class LCMWrapperPass : public llvm::PassInfoMixin<LCMWrapperPass> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &FAM) {

    /// @todo(CSCD70) Get the result from the main body.

    return llvm::PreservedAnalyses::all();
  }
};
