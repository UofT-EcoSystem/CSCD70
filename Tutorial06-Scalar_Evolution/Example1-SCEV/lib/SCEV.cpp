#include <vector>

#include <llvm/Analysis/ScalarEvolution.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

class SomePassThatUsesSCEV final : public PassInfoMixin<SomePassThatUsesSCEV> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
    ScalarEvolution &SCEVAnalysisResult =
        FAM.getResult<ScalarEvolutionAnalysis>(F);

    for (Instruction &I : instructions(F)) {
      if (const SCEV *SCEVOfI = SCEVAnalysisResult.getSCEV(&I)) {
        if (!isa<SCEVAddRecExpr>(SCEVOfI) && !isa<SCEVConstant>(SCEVOfI)) {
          continue;
        }
        outs() << "Instruction=" << I << " : " << *SCEVOfI << "\n";
      }
    }
    return PreservedAnalyses::all();
  }
};

extern "C" PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
      .APIVersion = LLVM_PLUGIN_API_VERSION,
      .PluginName = "SCEV",
      .PluginVersion = LLVM_VERSION_STRING,
      .RegisterPassBuilderCallbacks =
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) -> bool {
                  if (Name == "some-pass-that-uses-scev") {
                    FPM.addPass(SomePassThatUsesSCEV());
                    return true;
                  }
                  return false;
                });
          } // RegisterPassBuilderCallbacks
  };        // struct PassPluginLibraryInfo
}
