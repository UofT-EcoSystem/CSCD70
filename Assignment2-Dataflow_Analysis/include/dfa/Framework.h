#pragma once

#include <type_traits>
#include <unordered_set>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CFG.h>
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
/// @tparam TMeetOp         Meet Operator
/// @tparam TBC             Boundary Condition
template<typename TDomainElement, Direction TDirection,
         typename TMeetOp, typename TBC>
class Framework : public FunctionPass {

/**
 * @brief  Selectively enables methods depending on the analysis direction.
 * @param  dir  Direction of Analysis
 * @param  ret_type  Return Type
 */
#define METHOD_ENABLE_IF_DIRECTION(dir, ret_type)                               \
  template<Direction _TDirection = TDirection>                                  \
  typename std::enable_if_t<_TDirection == dir, ret_type>

/// This macro does typedef depending on the analysis direction. If the
/// direction in the template argument does not match the argument, the
/// resulting type is not defined.
///
/// @param type_name  Name of the Resulting Type
/// @param dir        Direction of Analysis
/// @param T          Type Definition if `TDirection == dir`
#define TYPEDEF_IF_DIRECTION(type_name, dir, T)                                 \
  using type_name = typename std::enable_if_t<TDirection == dir, T>

 protected:
  /// Domain
  std::unordered_set<TDomainElement> Domain;
  // Instruction-BitVector Mapping
  std::unordered_map<const Instruction*, BitVector> InstBVMap;
 private:
  TYPEDEF_IF_DIRECTION(MeetOpConstRange, Direction::kForward, const_pred_range);
  /*****************************************************************************
   * Auxiliary Print Subroutines
   *****************************************************************************/
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
      MeetOpConstRange meet_operands = MeetOperands(*InstParent);
      // If the list of meet operands is empty, then we are at the boundary,
      // hence print the BC.
      if (meet_operands.begin() == meet_operands.end()) {
        outs() << "BC:\t";
        TBC BC;
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
    PrintDomainWithMask(InstBVMap.at(&inst));
    outs() << "\n";
  }
  /*****************************************************************************
   * Meet Operator
   *****************************************************************************/
  METHOD_ENABLE_IF_DIRECTION(Direction::kForward, MeetOpConstRange)
  MeetOperands(const BasicBlock& bb) const {
    return predecessors(&bb);
  }

};
