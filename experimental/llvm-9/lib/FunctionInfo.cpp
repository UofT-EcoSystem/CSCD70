#include <llvm/IR/Function.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Pass.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>

using namespace llvm;

namespace {

struct HelloWorld : PassInfoMixin < HelloWorld > 
{
        PreservedAnalyses run(Function & F, 
                              FunctionAnalysisManager &)
        {
                outs() << "CSCD70 Functions Information Pass" << "\n";

                // // @TODO Please complete this function.

                return PreservedAnalyses::all();
        }
};

}  // namespace anonymous

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
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

// This is the core interface for pass plugins - with this 'opt' will be able
// to recognize HelloWorld when added to the pass pipeline on the command line,
// i.e. via '-passes=hello-world'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getHelloWorldPluginInfo();
}

//-----------------------------------------------------------------------------
// Legacy PM Registration
//-----------------------------------------------------------------------------
// The address of this variable is used to identify the pass. The actual value
// doesn't matter.
char LegacyHelloWorld::ID = 0;

// Register the pass - with this 'opt' will be able to recognize
// LegacyHelloWorld when added to the pass pipeline on the command line, i.e.
// via '--legacy-hello-world'
static RegisterPass<LegacyHelloWorld>
    X("legacy-hello-world", "Hello World Pass",
      true,  // This pass doesn't modify the CFG => true
      false  // This pass is not a pure analysis pass => false
    );


}  // namespace anonymous
