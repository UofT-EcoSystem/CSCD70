#include "DFA.h"

#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>

using namespace llvm;

extern "C" PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
      .APIVersion = LLVM_PLUGIN_API_VERSION,
      .PluginName = "DFA",
      .PluginVersion = LLVM_VERSION_STRING,
      .RegisterPassBuilderCallbacks =
          [](PassBuilder &PB) {
            PB.registerAnalysisRegistrationCallback(
                [](FunctionAnalysisManager &FAM) {
                  FAM.registerPass([&]() { return AvailExprs(); });
                  /// @todo(CSCD70) Please complete the registration of other
                  ///               passes.
                });
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) -> bool {
                  if (Name == "avail-expr") {
                    FPM.addPass(AvailExprsWrapperPass());
                    return true;
                  }
                  if (Name == "liveness") {
                    FPM.addPass(LivenessWrapperPass());
                    return true;
                  }
                  if (Name == "const-prop") {
                    FPM.addPass(SCCPWrapperPass());
                    return true;
                  }
                  if (Name == "lcm") {
                    FPM.addPass(LCMWrapperPass());
                    return true;
                  }
                  return false;
                });
          } // RegisterPassBuilderCallbacks
  };        // struct PassPluginLibraryInfo
}
