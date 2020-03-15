#pragma once

#include <dfa/framework.h>


// @TODO Implement `Dominators` using the `dfa::Framework` interface.
class Dominators final : public FunctionPass
{
public:
        static char ID;

        Dominators() : FunctionPass(ID) {}
        virtual ~Dominators() override {}

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
