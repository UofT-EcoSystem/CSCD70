#pragma once

#include <type_traits>
#include <unordered_map>

#include <llvm/Pass.h>
#include <llvm/ADT/BitVector.h>
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace dfa {
enum class Direction { Forward, Backward };

/// @brief Dataflow Analysis Framework
/// @tparam TDomain     Domain
/// @tparam TDirection  Analysis Direction
template < typename TDomain, Direction TDirection >
class Framework : public FunctionPass
{
protected:
        /***********************************************************************
         * Domain
         ***********************************************************************/
        TDomain _domain;  // Domain

        /// @brief Dump the domain under `mask`. E.g., If _domian={%1, %2, %3,},
        ///        dumping it with mask = 001 will give {%3,}.
        void dumpDomainWithMask(const BitVector & mask) const
        {
                outs() << "{";

                CHECK(mask.size() == _domain.size(),
                      "The size of mask must be equal to the size of domain.")
                
                for (unsigned mask_idx = 0; mask_idx < mask.size(); ++mask_idx)
                {
                        if (!mask[mask_idx])
                        {
                                continue;
                        }
                        outs() << _domain[mask_idx] << ", ";
                }  // for (mask_idx ∈ [0, mask.size()))
                outs() << "}";
        }
        /***********************************************************************
         * Instruction-BitVector Mapping
         ***********************************************************************/
        /// Mapping from Instruction Pointer to BitVector
        std::unordered_map < const Instruction *, BitVector > _inst_bv_map;
        /// @brief Return the boundary condition.
        virtual BitVector BC(const Function & func,
                             const BasicBlock & bb) const = 0;

        // Dump, for each Instruction in 'func', the associated bitvector.
        void dumpInstBVMap(const Function & func) const
        {
                outs() << "********************************************" << "\n";
                outs() << "* Instruction-BitVector Mapping             " << "\n";
                outs() << "********************************************" << "\n";

                for (const auto & bb : func)
                {
                        for (const auto & inst : bb)
                        {
                                const BitVector & bv = _inst_bv_map.at(&inst);
                                        
                                if (TDirection == Direction::Forward)
                                {
                                        if (&bb == &(func.getEntryBlock()) && 
                                            &inst == &(*bb.begin()))
                                        {
                                                outs() << "Boundary Condition: ";
                                                __dumpDomainWithMask(__getBoundaryCondition(func, bb));
                                                outs() << "\n";
                                        }

                                        outs() << "Instruction: " << inst << "\n";
                                        outs() << "\t"; __dumpDomainWithMask(bv); outs() << "\n";
                                }
                                else if (TDirection == Direction::Backward)
                                {
                                        outs() << "\t"; __dumpDomainWithMask(bv); outs() << "\n";
                                        outs() << "Instruction: " << inst << "\n";

                                        if (isa < ReturnInst > (inst) || 
                                            isa < UnreachableInst > (inst))
                                        {
                                                outs() << "Boundary Condition: ";
                                                __dumpDomainWithMask(__getBoundaryCondition(func, bb));
                                                outs() << "\n";
                                        }
                                }
                        }  // for (inst ∈ bb)
                }  // for (bb ∈ func)
        }

        /***********************************************************************
         * Meet Operator and Transfer Function
         ***********************************************************************/
        typedef std::conditional_t < TDirection == Direction::Forward,
                                     pred_const_range,
                                     succ_const_range > parent_const_range;
        virtual BitVector MeetOp(const parent_const_range & parents) = 0;
        /// @brief Instruction Transfer Function, to be implemented by child class.
        /// @param inst  Instruction
        virtual bool InstTransferFunc(const Instruction & inst, 
                                      const BitVector & ibv, BitVector & obv) = 0;

        /***********************************************************************
         * CFG Traversal
         ***********************************************************************/
        bool traverseCFG(const Function & func)
        {
                // @TODO
                return false;
        }
public:
        Framework(const char ID) : FunctionPass(ID) {}
        virtual ~Framework() override {}

        // We don't modify the program, so we preserve all analysis.
        virtual void getAnalysisUsage(AnalysisUsage & AU) const
        {
                AU.setPreservesAll();
        }

        virtual bool runOnFunction(Function & func) override final
        {
                TDomain domain;
                _initializeInstBVMap(func);

                bool is_convergent;
                do 
                {
                        is_convergent = true;

                        if (traverseCFG(func))
                        {
                                is_convergent = false;
                        }
                } while (!is_convergent);

                dumpInstBVMap(func);

                return false;
        }
};

}  // namespace dfa
