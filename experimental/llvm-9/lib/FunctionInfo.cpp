#include <llvm/Support/raw_stream.h>

using namespace llvm;

namespace {
struct HelloWorld : PassInfoMixin < HelloWorld > 
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

llvm::PassPluginLibraryInfo getHelloWorldPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "HelloWorld", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "hello-world") {
                    FPM.addPass(HelloWorld());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK 
llvm::PassPluginLibraryInfo llvmGetPassPluginInfo()
{
        return {

                };
}
