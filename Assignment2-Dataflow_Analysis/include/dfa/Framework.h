#ifndef DFA_FRAMEWORK_H
#define DFA_FRAMEWORK_H

#include <exception>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

#include "MeetOp.h"


namespace dfa {

/// Analysis Direction, used as Template Parameter
enum class Direction {
  kForward, kBackward
};

template<Direction TDirection>
struct FrameworkTypeSupport {};

template<>
struct FrameworkTypeSupport<Direction::kForward> {
  typedef const_pred_range MeetOpConstRange;
  typedef iterator_range<Function::const_iterator> BBTraversalConstRange;
  typedef iterator_range<BasicBlock::const_iterator> InstTraversalConstRange;
};
/**
 * @todo(cscd70) Please provide an equivalent instantiation for the backward pass.
 */

/**
 * @brief  Dataflow Analysis Framework
 *  
 * @tparam TDomainElement   Domain Element
 * @tparam TDomainElemRepr  Domain Element Representation (bool by Default)
 * @tparam TDirection       Direction of Analysis
 * @tparam TMeetOp          Meet Operator
 */
template<typename TDomainElement, typename TDomainElemRepr,
         Direction TDirection, typename TMeetOp>
class Framework {

/**
 * @brief  Selectively enables methods depending on the analysis direction.
 * @param  dir  Direction of Analysis
 * @param  ret_type  Return Type
 */
#define METHOD_ENABLE_IF_DIRECTION(dir, ret_type)                               \
  template<Direction _TDirection = TDirection>                                  \
  typename std::enable_if_t<_TDirection == dir, ret_type>

 private:
  using MeetOpConstRange
      = typename FrameworkTypeSupport<TDirection>::MeetOpConstRange;
  using BBTraversalConstRange
      = typename FrameworkTypeSupport<TDirection>::BBTraversalConstRange;
  using InstTraversalConstRange
      = typename FrameworkTypeSupport<TDirection>::InstTraversalConstRange;
 protected:
  /// Domain
  std::unordered_set<TDomainElement> Domain;
  // Instruction-BitVector Mapping
  std::unordered_map<const Instruction*, std::vector<TDomainElemRepr> > InstBVMap;
  /*****************************************************************************
   * Auxiliary Print Subroutines
   *****************************************************************************/
 private:
  /**
   * @brief  Print the domain with mask. E.g., If domian = {%1, %2, %3,},
   *         dumping it with mask = 001 will give {%3,}.
   */
  void printDomainWithMask(const std::vector<TDomainElemRepr>& Mask) const {
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
   * @todo(cscd70) Please provide an equivalent instantiation for the backward pass.
   */
  METHOD_ENABLE_IF_DIRECTION(Direction::kForward, void)
  printInstBV(const Instruction& Inst) const {
    const BasicBlock* const InstParent = Inst.getParent();
    if (&Inst == &(*InstParent->begin())) {
      MeetOpConstRange MeetOperands = getMeetOperands(*InstParent);
      // If the list of meet operands is empty, then we are at the boundary,
      // hence print the BC.
      if (MeetOperands.begin() == MeetOperands.end()) {
        outs() << "BC:\t";
        printDomainWithMask(bc());
        outs() << "\n";
      } else {
        outs() << "MeetOp:\t";
        printDomainWithMask(merge(MeetOperands));
        outs() << "\n";
      }
    }  // if (&inst == &(*InstParent->begin()))
    outs() << "Instruction: " << Inst << "\n";
    outs() << "\t";
    printDomainWithMask(InstBVMap.at(&Inst));
    outs() << "\n";
  }
  /**
   * @brief Dump, ∀inst ∈ @p 'F', the associated bitvector.
   */
  void printInstBVMap(const Function& F) const {
    outs() << "********************************************" << "\n"
           << "* Instruction-BitVector Mapping             " << "\n"
           << "********************************************" << "\n";
    for (const auto& Inst : instructions(F)) {
      printInstBV(Inst);
    }
  }
  /*****************************************************************************
   * Meet Operator
   *****************************************************************************/
  METHOD_ENABLE_IF_DIRECTION(Direction::kForward, MeetOpConstRange)
  getMeetOperands(const BasicBlock& BB) const {
    return predecessors(&BB);
  }
  /**
   * @brief  Apply the meet operator to the operands.
   * 
   * @todo(cscd70) Please complete the defintion of this method.
   */
  std::vector<TDomainElemRepr> merge(const MeetOpConstRange& MeetOperands) const {
    TMeetOp MeetOp;

    return std::vector<TDomainElemRepr>();
  }
  /*****************************************************************************
   * Transfer Function
   *****************************************************************************/
  /**
   * @brief  Apply the transfer function at instruction @c Inst to the input
   *         bitvector to get the output bitvector.
   * @return true if @c OBV has been changed, false otherwise
   * 
   * @todo(cscd70) Please implement this method for every child class.
   */
  virtual bool transferFunc(const Instruction& Inst,
                            const std::vector<TDomainElemRepr>& IBV,
                            std::vector<TDomainElemRepr>& OBV) = 0;
  /*****************************************************************************
   * CFG Traversal
   *****************************************************************************/
  /**
   * @brief  Boundary Condition
   */
  std::vector<TDomainElemRepr> bc() const {
    return std::vector<TDomainElemRepr>(Domain.size(), false);
  }
  /**
   * @brief  Return the traversal order of the basic blocks.
   */
  METHOD_ENABLE_IF_DIRECTION(Direction::kForward, BBTraversalConstRange)
  getBBTraversalOrder(const Function& F) const {
    return make_range(F.begin(), F.end());
  }
  /**
   * @brief  Return the traversal order of the instructions.
   * 
   * @todo(cscd70) Please modify the definition (and the above typedef accordingly)
   *               for the optimal traversal order.
   */
  METHOD_ENABLE_IF_DIRECTION(Direction::kForward, InstTraversalConstRange)
  getInstTraversalOrder(const BasicBlock& BB) const {
    return make_range(BB.begin(), BB.end());
  }
 protected:
  /**
   * @brief  Traverse through the CFG and update @c inst_bv_map .
   * @return true if changes are made to @c inst_bv_map , false otherwise
   * 
   * @todo(cscd70) Please implement this method for every child class.
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
  virtual void initializeDomain(const Function& F) {
    for (const auto& Inst : instructions(F)) {
      try {
        Domain.emplace(Inst);
      } catch (const std::invalid_argument& IA) {}
    }
    for (const auto& Arg : F.args()) {
      try {
        Domain.emplace(Arg);
      } catch (const std::invalid_argument& IA) {}
    }
  }
 public:
  virtual bool runOnFunction(Function& F) {
    // initialize the domain
    initializeDomain(F);
    // apply the initial conditions
    TMeetOp MeetOp;
    for (const auto& Inst : instructions(F)) {
      InstBVMap.emplace(&Inst, MeetOp.top());
    }
    // keep traversing until changes have been made to the instruction-bitvector mapping
    while (traverseCFG(F)) {}
    printInstBVMap(F);
    return false;
  }
};

}  // namespace dfa

#endif  // DFA_FRAMEWORK_H
