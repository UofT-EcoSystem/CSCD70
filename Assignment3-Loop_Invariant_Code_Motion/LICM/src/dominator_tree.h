#pragma once

#include <dfa/framework.h>


/// @todo Implement @c DominatorTree using the @c dfa::Framework interface.
class DominatorTree final : public FunctionPass
{
public:
        static char ID;

        DominatorTree() : FunctionPass(ID) {}
        virtual ~DominatorTree() override {}

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
