#include "LocalOpts.h"

using namespace llvm;

extern "C" PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
      .APIVersion = LLVM_PLUGIN_API_VERSION,
      .PluginName = "LocalOpts",
      .PluginVersion = LLVM_VERSION_STRING,
      .RegisterPassBuilderCallbacks =
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) -> bool {
                  if (Name == "algebraic-identity") {
                    FPM.addPass(AlgebraicIdentityPass());
                    return true;
                  }
                  if (Name == "strength-reduction") {
                    FPM.addPass(StrengthReductionPass());
                    return true;
                  }
                  if (Name == "multi-inst-opt") {
                    FPM.addPass(MultiInstOptPass());
                    return true;
                  }
                  return false;
                });
          } // RegisterPassBuilderCallbacks
  };        // struct PassPluginLibraryInfo
}
