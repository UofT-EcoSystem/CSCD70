#include <llvm/Pass.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace {
struct FunctionInfo : PassInfoMixin < FunctionInfo > 
{
        PreservedAnalyses run(Function & F, 
                              FunctionAnalysisManager &)
        {
                outs() << "CSCD70 Functions Information Pass" << "\n";

                // @TODO Please complete this function.

                return PreservedAnalyses::all();
        }
};
}  // namespace anonymous

extern "C" 
llvm::PassPluginLibraryInfo llvmGetPassPluginInfo()
{
        return {.APIVersion = LLVM_PLUGIN_API_VERSION,
                .PluginName = "FunctionInfo",
                .PluginVersion = LLVM_VERSION_STRING,
                .RegisterPassBuilderCallbacks = [](PassBuilder & PB) {
                        PB.registerPipelineParsingCallback(
                                [](StringRef Name, FunctionPassManager & FPM,
                                ArrayRef < PassBuilder::PipelineElement > )
                                {
                                        if (Name == "function-info")
                                        {
                                                FPM.addPass(FunctionInfo());
                                                return true;
                                        }
                                        return false;
                                });
                        }
                };
}
