#include "dfa/framework.h"

#include <llvm/PassSupport.h>

using namespace llvm;


namespace {

// @TODO Implement `Liveness` using the `dfa::Framework` interface.
class Liveness final : public FunctionPass
{
public:
        static char ID;

        Liveness() : FunctionPass(ID) {}
        virtual ~Liveness() override {}

        // @TODO Add or remove method definitions if necessary.
        
        virtual void getAnalysisUsage(AnalysisUsage & AU) const override
        {
                AU.setPreservesAll();
        }

        virtual bool runOnFunction(Function & func) override
        {
                return false;
        }
};

char Liveness::ID = 1; 
RegisterPass < Liveness > Y ("liveness", "Liveness");

} // namespace anonymous
