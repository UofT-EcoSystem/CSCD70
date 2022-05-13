#pragma once // NOLINT(llvm-header-guard)

#include <llvm/Pass.h>

using namespace llvm;

class Analysis final : public ModulePass {
private:
  std::vector<unsigned> Stats;

public:
  static char ID;

  Analysis() : ModulePass(ID) {}

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override;
  virtual bool runOnModule(Module &M) override;

  /**
   * @brief Return the gathered statstics.
   */
  std::vector<unsigned> getStats() const { return Stats; }
};
