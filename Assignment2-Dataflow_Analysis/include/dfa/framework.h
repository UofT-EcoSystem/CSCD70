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
struct FrameworkMetaData {};

template <>
struct FrameworkMetaData < Direction::Forward >
{
        typedef pred_const_range  meetop_const_range;
        typedef iterator_range < Function::const_iterator >
                bb_traversal_const_range;
        typedef iterator_range < BasicBlock::const_iterator >
                inst_traversal_const_range;
};

/// Dataflow Analysis Framework
/// 
/// @tparam TDomainElement  Domain Element
/// @tparam TDirection      Direction of Analysis
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
                        meetop_const_range meet_operands = MeetOperands(*pbb);
                        // If the list of meet operands is empty, then we are at
                        // the boundary, hence print the BC.
                        if (meet_operands.begin() == meet_operands.end())
                        {
                                outs() << "BC:\t";
                                printDomainWithMask(BC());
                                outs() << "\n";
                        }
                        else
                        {
                                outs() << "MeetOp:\t";
                                printDomainWithMask(MeetOp(meet_operands));
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
        void printInstBVMap(const Function & F) const
        {
                outs() << "********************************************" << "\n";
                outs() << "* Instruction-BitVector Mapping             " << "\n";
                outs() << "********************************************" << "\n";

                for (const auto & inst : instructions(F))
                {
                        printInstBV(inst);
                }
        }
        /***********************************************************************
         * Meet Operator and Transfer Function
         ***********************************************************************/
        using meetop_const_range =
                typename FrameworkMetaData < TDirection > ::
                        meetop_const_range;
        /// @brief Return the operands for the MeetOp.
        METHOD_ENABLE_IF_DIRECTION(Direction::Forward, meetop_const_range)
        MeetOperands(const BasicBlock & bb) const
        {
                return predecessors(&bb);
        }
        /// @brief Apply the meet operation to a range of operands.
        /// 
        /// @return the Resulting BitVector after the Meet Operation
        virtual BitVector MeetOp(const meetop_const_range & meet_operands) const = 0;
        /// @brief Apply the instruction transfer function to the input
        ///        bitvector `ibv` to obtain the output bitvector `obv`.
        /// 
        /// @return true if `obv` has been changed, false otherwise
        virtual bool TransferFunc(const Instruction & inst, 
                                  const BitVector & ibv,
                                  BitVector & obv) = 0;
        /***********************************************************************
         * CFG Traversal
         ***********************************************************************/
private:
        using bb_traversal_const_range =
                typename FrameworkMetaData < TDirection > ::
                        bb_traversal_const_range;
        using inst_traversal_const_range =
                typename FrameworkMetaData < TDirection > ::
                        inst_traversal_const_range;
        /// @brief Return the traversal order of the basic blocks.
        METHOD_ENABLE_IF_DIRECTION(Direction::Forward, bb_traversal_const_range)
        BBTraversalOrder(const Function & F) const
        {
                return make_range(F.begin(), F.end());
        }
        /// @brief Return the traversal order of the instructions.
        METHOD_ENABLE_IF_DIRECTION(Direction::Forward, inst_traversal_const_range)
        InstTraversalOrder(const BasicBlock & bb) const
        {
                return make_range(bb.begin(), bb.end());
        }
protected:
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

        virtual bool runOnFunction(Function & F) override final
        {
                for (const auto & inst : instructions(F))
                {
                        _domain.emplace(inst);
                }
                for (const auto & inst : instructions(F))
                {
                        _inst_bv_map.emplace(&inst, IC());
                }
                bool is_convergent;
                do 
                {
                        is_convergent = true;

                        if (traverseCFG(F))
                        {
                                is_convergent = false;
                        }
                } while (!is_convergent);

                printInstBVMap(F);

                return false;
        }
#undef METHOD_ENABLE_IF_DIRECTION
};

}  // namespace dfa
