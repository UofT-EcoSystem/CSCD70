#pragma once

#include <type_traits>
#include <unordered_set>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;


/// Analysis Direction, used as Template Parameter
enum class Direction {
  kForward, kBackward
};

/// @brief  Dataflow Analysis Framework
/// 
/// @tparam TDomainElement  Domain Element
/// @tparam TDirection      Direction of Analysis
template<typename TDomainElement, Direction TDirection>
class Framework : public FunctionPass {

/**
 * @brief  Selectively enables methods depending on the analysis direction.
 * @param  dir  Direction of Analysis
 * @param  ret_type  Return Type
 */
#define METHOD_ENABLE_IF_DIRECTION(dir, ret_type)                               \
  template<Direction _TDirection = TDirection>                                  \
  typename std::enable_if_t<_TDirection == dir, ret_type>

 protected:
  /// Domain
  std::unordered_set<TDomainElement> Domain;
 private:
  /**
   * @brief  Print the domain with mask. E.g., If domian = {%1, %2, %3,},
   *         dumping it with mask = 001 will give {%3,}.
   */
  void PrintDomainWithMask(const BitVector& mask) const {
    outs() << "{";
    assert(mask.size() == Domain.size() &&
           "The size of mask must be equal to the size of domain.");
    unsigned mask_idx = 0;
    for (const auto& elem : Domain) {
      if (!mask[mask_idx++]) {
        continue;
      }
      outs() << elem << ", ";
    }  // for (mask_idx ∈ [0, mask.size()))
    outs() << "}";
  }
  /**
   * @todo(cscd70) Provide an equivalent instantiation for the backward pass.
   */
  METHOD_ENABLE_IF_DIRECTION(Direction::kForward, void)
  PrintInstBV(const Instruction& inst) const {
    const BasicBlock* const InstParent = inst.getParent();
    if (&inst == &(*InstParent->begin())) {
      typename TMeetOp::MeetOpConstRange meet_operands = MeetOperands(*InstParent);
      // If the list of meet operands is empty, then we are at the boundary,
      // hence print the BC.
      if (meet_operands.begin() == meet_operands.end()) {
        outs() << "BC:\t";
        PrintDomainWithMask(BC());
        outs() << "\n";
      } else {
        outs() << "MeetOp:\t";
        PrintDomainWithMask(MeetOp(meet_operands));
        outs() << "\n";
      }
    }  // if (&inst == &(*InstParent->begin()))
    outs() << "Instruction: " << inst << "\n";
    outs() << "\t";
    PrintDomainWithMask(_inst_bv_map.at(&inst));
    outs() << "\n";
  }
};
