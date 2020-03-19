#include <llvm/Analysis/LoopPass.h>
#include <llvm/Analysis/ValueTracking.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;


namespace {

class LoopInvariantCodeMotion final : public LoopPass
{
private:
	DominatorTree * dom_tree;  // owned by `DominatorTreeWrapperPass`
public:
        static char ID;

	LoopInvariantCodeMotion() : LoopPass(ID) {}
	virtual ~LoopInvariantCodeMotion() override {}

        virtual void getAnalysisUsage(AnalysisUsage & AU) const
	{
		AU.addRequired < DominatorTreeWrapperPass > ();
		AU.setPreservesCFG();
	}

	/// @todo Finish the implementation of this method.
        virtual bool runOnLoop(Loop * L, LPPassManager & LPM)
	{
		dom_tree = &(getAnalysis < DominatorTreeWrapperPass > ().getDomTree());

		// @TODO

                return false;
        }
};

char LoopInvariantCodeMotion::ID = 0;

RegisterPass < LoopInvariantCodeMotion > X (
	"loop-invariant-code-motion",
	"Loop Invariant Code Motion");

}  // namespace anonymous
