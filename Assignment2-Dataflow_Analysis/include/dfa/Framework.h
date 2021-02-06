#ifndef DFA_FRAMEWORK_H
#define DFA_FRAMEWORK_H

#include <type_traits>
#include <unordered_map>
#include <unordered_set>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;


namespace dfa {

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
class Framework {

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
  TYPEDEF_IF_DIRECTION(BBTraversalConstRange, Direction::kForward,
                       iterator_range<Function::const_iterator>);
  TYPEDEF_IF_DIRECTION(InstTraversalConstRange, Direction::kForward,
                       iterator_range<BasicBlock::const_iterator>);
  /*****************************************************************************
   * Auxiliary Print Subroutines
   *****************************************************************************/
 private:
  /**
   * @brief  Print the domain with mask. E.g., If domian = {%1, %2, %3,},
   *         dumping it with mask = 001 will give {%3,}.
   */
  void printDomainWithMask(const BitVector& Mask) const {
    outs() << "{";
    assert(Mask.size() == Domain.size() &&
           "The size of mask must be equal to the size of domain.");
    unsigned MaskIdx = 0;
    for (const auto& Elem : Domain) {
      if (!Mask[MaskIdx++]) {
        continue;
      }
      outs() << Elem << ", ";
    }  // for (MaskIdx ∈ [0, Mask.size()))
    outs() << "}";
  }
  /**
   * @todo(cscd70) Provide an equivalent instantiation for the backward pass.
   */
  METHOD_ENABLE_IF_DIRECTION(Direction::kForward, void)
  printInstBVMap(const Instruction& Inst) const {
    const BasicBlock* const InstParent = Inst.getParent();
    if (&Inst == &(*InstParent->begin())) {
      MeetOpConstRange MeetOperands = getMeetOperands(*InstParent);
      // If the list of meet operands is empty, then we are at the boundary,
      // hence print the BC.
      if (MeetOperands.begin() == MeetOperands.end()) {
        outs() << "BC:\t";
        TBC BC;
        printDomainWithMask(BC());
        outs() << "\n";
      } else {
        outs() << "MeetOp:\t";
        TMeetOp MeetOp;
        printDomainWithMask(MeetOp(MeetOperands));
        outs() << "\n";
      }
    }  // if (&inst == &(*InstParent->begin()))
    outs() << "Instruction: " << Inst << "\n";
    outs() << "\t";
    printDomainWithMask(InstBVMap.at(&Inst));
    outs() << "\n";
  }
  /*****************************************************************************
   * Meet Operator
   *****************************************************************************/
  METHOD_ENABLE_IF_DIRECTION(Direction::kForward, MeetOpConstRange)
  getMeetOperands(const BasicBlock& BB) const {
    return predecessors(&BB);
  }

 protected:
  /**
   * @brief  Traverse through the CFG and update @c inst_bv_map .
   * @return true if changes are made to @c inst_bv_map, false otherwise
   */
  bool traverseCFG(const Function& F) {
    return false;
  }
 public:
  virtual ~Framework() {}

  // We don't modify the program, so we preserve all analysis.
  virtual void getAnalysisUsage(AnalysisUsage& AU) const {
    AU.setPreservesAll();
  }
 protected:
  /**
   * @brief Initialize the domain from each instruction.
   */
  virtual void initializeDomainFromInstruction(const Instruction& Inst) = 0;
 public:
  virtual bool runOnFunction(Function &F) {
    for (const auto& Inst : instructions(F)) {
      initializeDomainFromInstruction(Inst);
    }
    TMeetOp MeetOp;
    for (const auto& Inst : instructions(F)) {
      InstBVMap.emplace(&Inst, MeetOp.IC());
    }
    // keep traversing until changes have been made to the instruction-bitvector mapping
    while (traverseCFG(F)) {}
    printInstBVMap(F);
    return false;
  }
};

}  // namespace dfa

#endif  // DFA_FRAMEWORK_H
