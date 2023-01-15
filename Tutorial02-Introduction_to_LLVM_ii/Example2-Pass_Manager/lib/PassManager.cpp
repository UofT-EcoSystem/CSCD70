#include <vector>

#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

/**
 * @brief An analysis pass that hypothetically gathers some information from the
 *        module.
 */
class SomeAnalysis final : public AnalysisInfoMixin<SomeAnalysis> {
public:
  /**
   * @brief The information that is collected by `SomeAnalysis`. In this
   *        specific example we are assuming it to be of type vector.
   */
  struct Result : std::vector<unsigned> {
    /**
     * @brief Verify the validity of the analysis results. This method is
     *        implicitly invoked by the pass manager.
     */
    bool invalidate(Module &, const PreservedAnalyses &PA,
                    ModuleAnalysisManager::Invalidator &) {
      PreservedAnalyses::PreservedAnalysisChecker PAC =
          PA.getChecker<SomeAnalysis>();
      bool ResIsNotValid =
          !(PAC.preserved() || PAC.preservedSet<AllAnalysesOn<Module>>());
      outs() << "Verifying the validity of the analysis results: "
             << (ResIsNotValid ? 'N' : 'Y') << "\n";
      return ResIsNotValid;
    }
  };

private:
  friend AnalysisInfoMixin<SomeAnalysis>;
  static AnalysisKey Key;

public:
  Result run(Module &, ModuleAnalysisManager &) {
    outs() << "Some Analysis"
           << "\n";
    Result SomeAnalysisResults;
    // pretend as if we have gathered some information here
    SomeAnalysisResults.push_back(1);
    SomeAnalysisResults.push_back(2);
    SomeAnalysisResults.push_back(3);
    return SomeAnalysisResults;
  }
};

AnalysisKey SomeAnalysis::Key;

class SomeTransformThatPreserves final
    : public PassInfoMixin<SomeTransformThatPreserves> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM) {
    outs() << "Some Transform that preserves"
           << "\n";
    SomeAnalysis::Result &SomeAnalysisResult = MAM.getResult<SomeAnalysis>(M);
    for (const unsigned Res : SomeAnalysisResult) {
      outs() << Res << ", ";
    }
    outs() << "\n";
    // indicate that we are preserving `SomeAnalysis`
    PreservedAnalyses PA = PreservedAnalyses::none();
    PA.preserve<SomeAnalysis>();
    return PA;
  }
};

class SomeTransformThatDoesNOTPreserve final
    : public PassInfoMixin<SomeTransformThatDoesNOTPreserve> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM) {
    outs() << "Some Transform that does NOT preserve"
           << "\n";
    SomeAnalysis::Result &SomeAnalysisResult = MAM.getResult<SomeAnalysis>(M);
    for (const unsigned Res : SomeAnalysisResult) {
      outs() << Res << ", ";
    }
    outs() << "\n";
    // indicate that we are abandoning (i.e., not preserving) `SomeAnalysis`
    PreservedAnalyses PA = PreservedAnalyses::all();
    PA.abandon<SomeAnalysis>();
    return PA;
  }
};

class AnotherTransform final : public PassInfoMixin<AnotherTransform> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM) {
    outs() << "Another Transform"
           << "\n";
    SomeAnalysis::Result &SomeAnalysisResult = MAM.getResult<SomeAnalysis>(M);
    for (const unsigned Res : SomeAnalysisResult) {
      outs() << Res << ", ";
    }
    outs() << "\n";
    return PreservedAnalyses::none();
  }
};

extern "C" PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
      .APIVersion = LLVM_PLUGIN_API_VERSION,
      .PluginName = "PassManager",
      .PluginVersion = LLVM_VERSION_STRING,
      .RegisterPassBuilderCallbacks =
          [](PassBuilder &PB) {
            // register the analysis pass so that it can be requested by others
            PB.registerAnalysisRegistrationCallback(
                [](ModuleAnalysisManager &MAM) {
                  MAM.registerPass([&]() { return SomeAnalysis(); });
                });
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM,
                   ArrayRef<PassBuilder::PipelineElement>) -> bool {
                  if (Name == "some-transform-that-preserves") {
                    MPM.addPass(SomeTransformThatPreserves());
                    return true;
                  }
                  if (Name == "some-transform-that-does-NOT-preserve") {
                    MPM.addPass(SomeTransformThatDoesNOTPreserve());
                    return true;
                  }
                  if (Name == "another-transform") {
                    MPM.addPass(AnotherTransform());
                    return true;
                  }
                  return false;
                });
          } // RegisterPassBuilderCallbacks
  };        // struct PassPluginLibraryInfo
}
