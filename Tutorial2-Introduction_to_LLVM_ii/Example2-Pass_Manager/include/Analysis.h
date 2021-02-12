#pragma once

#include <llvm/Pass.h>

using namespace llvm;

class Analysis final : public ModulePass {
private:
  std::vector<unsigned> Stats_;

public:
  static char ID;

  Analysis() : ModulePass(ID) {}
  virtual ~Analysis() override {}

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override;
  virtual bool runOnModule(Module &M) override;

  // Return the gathered statstics.
  std::vector<unsigned> getStats() const { return Stats_; }
};
