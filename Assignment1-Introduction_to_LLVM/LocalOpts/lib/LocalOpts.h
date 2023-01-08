#pragma once

#include <llvm/Pass.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>

class AlgebraicIdentityPass final
    : public llvm::PassInfoMixin<AlgebraicIdentityPass> {
public:
  llvm::PreservedAnalyses run(llvm::Function &M,
                              llvm::FunctionAnalysisManager &);
}; // class AlgebraicIdentityPass

class StrengthReductionPass final
    : public llvm::PassInfoMixin<StrengthReductionPass> {
public:
  llvm::PreservedAnalyses run(llvm::Function &M,
                              llvm::FunctionAnalysisManager &);
}; // class StrengthReductionPass

class MultiInstOptPass final : public llvm::PassInfoMixin<MultiInstOptPass> {
public:
  llvm::PreservedAnalyses run(llvm::Function &M,
                              llvm::FunctionAnalysisManager &);
}; // class MultiInstOptPass
