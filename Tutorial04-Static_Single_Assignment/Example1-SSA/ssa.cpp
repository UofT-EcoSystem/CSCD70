#include <llvm/Pass.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstVisitor.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Analysis/DominanceFrontier.h>

using namespace llvm;

namespace {
class SSADemo : public ModulePass
{
private:
        
        struct PhiNodeVisitor : public InstVisitor < PhiNodeVisitor >
        {
                void visitPHINode(PHINode & PhiInst)
                {
                        outs() << "I am PHI Node: " << PhiInst << "\n"
                        outs() << "\t" "I am in Basic Block: ";
                        PhiInst.getParent()->printAsOperand(outs(), false);
                        outs() << "\n";
                        outs() << "\t" "The Nodes that I have are:" "\n";
                        for (unsigned node_idx = 0; node_idx < PhiInst.getNumIncomingValues(); ++node_idx)
                        {
                                outs() << "\t\t" "Value: ";
                                PhiInst.getIncomingValue(node_idx)->printAsOperand(outs(), false);
                                outs() << " <- " "BB: ";
                                PhiInst.getIncomingBlock(node_idx)->printAsOperand(outs(), false);
                                outs() << "\n";
                        }
                }
        } _phi_node_visitor;
        
        bool runOnBasicBlock(BasicBlock & B, DominanceFrontier & dom_frontier)
        {
                outs() << "*****************************************************" "\n";
                
                for (auto iter = B.begin(); iter != B.end(); ++iter)
                {
                        _phi_node_visitor.visit(*iter);
                }
                
                DominanceFrontier::iterator dom_frontier_iter = dom_frontier.find(&B);
                
                assert(dom_frontier_iter != dom_frontier.end() && 
                       "BasicBlock much exist in the list of Dominance Frontier.");
                
                outs() << "Dominance Frontier: " << "{";
                for (auto df_set_iter  = dom_frontier_iter->second.begin();
                          df_set_iter != dom_frontier_iter->second.end(); 
                        ++df_set_iter)
                {
                        (*df_set_iter)->printAsOperand(outs(), false); outs() << ", ";
                }
                outs() << "}" "\n";

                return false;
        }
        
        bool runOnFunction(Function & F)
        {
                bool transformed = false;
                        
                DominanceFrontier & dom_frontier = 
                        getAnalysis < DominanceFrontierWrapperPass > (F)
                                .getDominanceFrontier();
                
                for (auto iter = F.begin(); iter != F.end(); ++iter)
                {
                        if (runOnBasicBlock(*iter, dom_frontier))
                        {
                                transformed = true;
                        }
                }
                return transformed;
        }
        
public:
        static char ID;

        SSADemo() : ModulePass(ID) {}
        virtual ~SSADemo() {}

        virtual void getAnalysisUsage(AnalysisUsage & AU) const
        {
                AU.addRequired < DominanceFrontierWrapperPass > (); 
                AU.setPreservesAll();
        }
  
        virtual bool runOnModule(Module & M)
        {
                bool transformed = false;
                
                for (auto iter = M.begin(); iter != M.end(); ++iter)
                {
                        if (runOnFunction(*iter))
                        {
                                transformed = true;
                        }
                }
                
                return transformed;
        }
};

char SSADemo::ID = 0;
RegisterPass < SSADemo > X ("ssa", "SSA Demo");

}  // namespace anonymous
