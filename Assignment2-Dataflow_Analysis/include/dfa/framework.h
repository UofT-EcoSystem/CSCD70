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
/// Analysis Direction, used as Template Parameter
enum class Direction { Forward, Backward };

/// Dataflow Analysis Framework
/// 
/// @tparam TDomainElement  Domain Element
/// @tparam TDirection      Analysis Direction
template < typename TDomainElement, Direction TDirection >
class Framework : public FunctionPass
{
/// This macro selectively enables methods depending the direction of analysis.
/// 
/// @param dir  Direction of Analysis
/// @param ret  Return Type
#define METHOD_ENABLE_IF_DIRECTION(dir, ret)                                    \
        template < _TDirection = TDirection >                                   \
        std::enable_if_t < _TDirection == dir, ret >
protected:
        /***********************************************************************
         * Domain
         ***********************************************************************/
        std::unordered_set < TDomainElement > _domain;
        /***********************************************************************
         * Instruction-BitVector Mapping
         ***********************************************************************/
        /// Mapping from Instruction Pointer to BitVector
        std::unordered_map < const Instruction *, BitVector > _inst_bv_map;
        /// @brief Return the initial condition.
        virtual BitVector IC() const = 0;
        /// @brief Return the boundary condition.
        virtual BitVector BC() const = 0;
private:
        /// @brief Dump the domain under `mask`. E.g., If `_domian`={%1, %2,
        ///        %3,}, dumping it with `mask`=001 will give {%3,}.
        void printDomainWithMask(const BitVector & mask) const
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
        METHOD_ENABLE_IF_DIRECTION(Direction::Forward, void)
        printInstBV(const Function & func,
                    const BasicBlock & bb,
                    const Instruction & inst)
        {
                if (&inst == &(*bb.begin()))
                {
                        if (&bb == &(func.getEntryBlock()))
                        {
                                outs() << "BC:\t";
                                printDomainWithMask(BC());
                                outs() << "\n";
                        }
                        else
                        {
                                outs() << "MeetOp:\t";
                                // @TODO
                                outs() << "\n";
                        }
                }
                outs() << "Instruction: " << inst << "\n";
                outs() << "\t";
                printDomainWithMask(bv);
                outs() << "\n";
        }
        METHOD_ENABLE_IF_DIRECTION(Direction::Forward, void)
        printInstBV(const Function & func,
                    const BasicBlock & bb,
                    const Instruction & inst)
        {
                outs() << "\t";
                printDomainWithMask(_inst_bv_map.at(&inst));
                outs() << "\n";
                outs() << "Instruction: " << inst << "\n";

                if (isa < ReturnInst > (inst) || 
                    isa < UnreachableInst > (inst))
                {
                        outs() << "BC:\t";
                        printDomainWithMask(BC());
                        outs() << "\n";
                }
                else if (&inst == &(*bb.rbegin()))
                {
                        outs() << "MeetOp:\t";
                        // @TODO
                        outs() << "\n";
                }
        }
protected:
        /// @brief Dump, for each Instruction in 'func', the associated bitvector.
        void printInstBVMap(const Function & func) const
        {
                outs() << "********************************************" << "\n";
                outs() << "* Instruction-BitVector Mapping             " << "\n";
                outs() << "********************************************" << "\n";

                for (const auto & bb : func)
                {
                        for (const auto & inst : bb)
                        {
                                printInstBV(func, bb, inst);                                
                        }  // for (inst ∈ bb)
                }  // for (bb ∈ func)
        }
        /***********************************************************************
         * Meet Operator and Transfer Function
         ***********************************************************************/
        template < typename _TDirection > struct parent_const_range {};
        template <>
        struct parent_const_range < Direction::Forward > 
        { 
                typedef pred_const_range type;
        };
        template < typename _TDirection >
        typedef parent_const_range < _TDirection > ::type parent_const_range_t;

        virtual BitVector MeetOp(const ParentConstRange < TDirection > & parents) = 0;
        /// @brief Instruction Transfer Function, to be implemented by child class.
        /// 
        /// @param inst  Instruction
        /// @param ibv   Input BitVector
        /// @param obv   Output BitVector
        /// @return true if `obv` has been changed, false otherwise
        virtual bool InstTransferFunc(const Instruction & inst, 
                                      const BitVector & ibv,
                                      BitVector & obv) = 0;
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
                _domain = TDomain(func);
                for (const auto & bb : func)
                {
                        for (const auto & inst : bb)
                        {
                                _inst_bv_map.insert(std::make_pair(
                                        &inst, IC()));
                        }
                }

                bool is_convergent;
                do 
                {
                        is_convergent = true;

                        if (traverseCFG(func))
                        {
                                is_convergent = false;
                        }
                } while (!is_convergent);

                printInstBVMap(func);

                return false;
        }
#undef METHOD_ENABLE_IF_DIRECTION
};

}  // namespace dfa
