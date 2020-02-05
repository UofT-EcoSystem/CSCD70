#pragma once

#include <cassert>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

#include <llvm/Pass.h>
#include <llvm/ADT/BitVector.h>
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace dfa {
/// Analysis Direction, used as Template Parameter
enum class Direction { Forward, Backward };

template < Direction TDirection >
struct meetop_const_range {};

template <>
struct meetop_const_range < Direction::Forward > 
{ 
        typedef pred_const_range type;
};

// @TODO

template < Direction TDirection >
using meetop_const_range_t = typename meetop_const_range < TDirection > ::type;

template < Direction TDirection >
struct traversal_order_const_iter

template 

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
        template < Direction _TDirection = TDirection >                         \
        std::enable_if_t < _TDirection == dir, ret >
protected:
        typedef TDomainElement domain_element_t;
        static constexpr Direction direction_c = TDirection;
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

                assert(mask.size() == _domain.size() &&
                       "The size of mask must be equal to the size of domain.");
                
                unsigned mask_idx = 0;
                for (const auto & elem : _domain)
                {
                        if (!mask[mask_idx++])
                        {
                                continue;
                        }
                        outs() << elem << ", ";
                }  // for (mask_idx ∈ [0, mask.size()))
                outs() << "}";
        }
        METHOD_ENABLE_IF_DIRECTION(Direction::Forward, void)
        printInstBV(const Instruction & inst) const
        {
                const BasicBlock * const pbb = inst.getParent();

                if (&inst == &(*pbb->begin()))
                {
                        // If the predecessors of `bb` is empty, then we are at
                        // the function entry, hence print the BC.
                        if (pred_empty(pbb))
                        {
                                outs() << "BC:\t";
                                printDomainWithMask(BC());
                                outs() << "\n";
                        }
                        else
                        {
                                outs() << "MeetOp:\t";
                                printDomainWithMask(MeetOp(MeetOperands(*pbb)));
                                outs() << "\n";
                        }
                }
                outs() << "Instruction: " << inst << "\n";
                outs() << "\t";
                printDomainWithMask(_inst_bv_map.at(&inst));
                outs() << "\n";
        }
protected:
        /// @brief Dump, for each Instruction in 'func', the associated bitvector.
        void printInstBVMap(const Function & func) const
        {
                outs() << "********************************************" << "\n";
                outs() << "* Instruction-BitVector Mapping             " << "\n";
                outs() << "********************************************" << "\n";

                for (const auto & inst : instructions(func))
                {
                        printInstBV(inst);
                }
        }
        /***********************************************************************
         * Meet Operator and Transfer Function
         ***********************************************************************/
        /// @brief Return the operands for the MeetOp.
        METHOD_ENABLE_IF_DIRECTION(Direction::Forward, meetop_const_range_t < TDirection >)
        MeetOperands(const BasicBlock & bb) const
        {
                return predecessors(&bb);
        }
        /// @brief Apply the meet operation to a range of operands.
        /// 
        /// @return the Resulting BitVector after the Meet Operation
        virtual BitVector MeetOp(const meetop_const_range_t < TDirection > & parents) const = 0;
        /// @brief Apply the instruction transfer function to the input
        ///        bitvector `ibv` to obtain the output bitvector `obv`.
        /// 
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
        Framework(char ID) : FunctionPass(ID) {}
        virtual ~Framework() override {}

        // We don't modify the program, so we preserve all analysis.
        virtual void getAnalysisUsage(AnalysisUsage & AU) const
        {
                AU.setPreservesAll();
        }

        virtual bool runOnFunction(Function & func) override final
        {
                for (const auto & inst : instructions(func))
                {
                        _domain.emplace(inst);
                }
                for (const auto & inst : instructions(func))
                {
                        _inst_bv_map.emplace(&inst, IC());
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
