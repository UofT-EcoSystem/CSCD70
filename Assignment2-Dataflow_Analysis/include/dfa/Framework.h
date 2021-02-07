#pragma once // NOLINT(llvm-header-guard)

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
enum class Direction { kForward, kBackward };

template <Direction TDirection> //
struct FrameworkTypeSupport {};

/**
 * @todo(cscd70) Modify the typedefs if necessary.
 */
template <> //
struct FrameworkTypeSupport<Direction::kForward> {
  typedef const_pred_range MeetOpConstRange;
  typedef iterator_range<Function::const_iterator> BBTraversalConstRange;
  typedef iterator_range<BasicBlock::const_iterator> InstTraversalConstRange;
};

/**
 * @todo(cscd70) Please provide an instantiation for the backward pass.
 */

/**
 * @brief  Dataflow Analysis Framework
 *
 * @tparam TDomainElem      Domain Element
 * @tparam TDomainElemRepr  Domain Element Representation
 * @tparam TDirection       Direction of Analysis
 * @tparam TMeetOp          Meet Operator
 */
template <typename TDomainElem, typename TDomainElemRepr, Direction TDirection,
          typename TMeetOp>
class Framework {

/**
 * @brief Selectively enables methods depending on the analysis direction.
 * @param dir  Direction of Analysis
 * @param ret_type  Return Type
 */
#define METHOD_ENABLE_IF_DIRECTION(dir, ret_type)                              \
  template <Direction _TDirection = TDirection>                                \
  typename std::enable_if_t<_TDirection == dir, ret_type>

private:
  using MeetOpConstRange =
      typename FrameworkTypeSupport<TDirection>::MeetOpConstRange;
  using BBTraversalConstRange =
      typename FrameworkTypeSupport<TDirection>::BBTraversalConstRange;
  using InstTraversalConstRange =
      typename FrameworkTypeSupport<TDirection>::InstTraversalConstRange;

protected:
  /// Domain
  std::unordered_set<TDomainElem> Domain;
  // Instruction-Domain Mapping
  std::unordered_map<const Instruction *, //
                     std::vector<TDomainElemRepr>>
      InstDomainMap;
  /*****************************************************************************
   * Auxiliary Print Subroutines
   *****************************************************************************/
private:
  /**
   * @brief Print the domain with mask. E.g., If domian = {%1, %2, %3,},
   *        dumping it with mask = 001 will give {%3,}.
   */
  void printDomainWithMask(const std::vector<TDomainElemRepr> &Mask) const {
    outs() << "{";
    assert(Mask.size() == Domain.size() &&
           "The size of mask must be equal to the size of domain.");
    unsigned MaskIdx = 0;
    for (const auto &Elem : Domain) {
      if (!Mask[MaskIdx++]) {
        continue;
      }
      outs() << Elem << ", ";
    } // for (MaskIdx ∈ [0, Mask.size()))
    outs() << "}";
  }
  /**
   * @todo(cscd70) Please provide an instantiation for the backward pass.
   */
  METHOD_ENABLE_IF_DIRECTION(Direction::kForward, void)
  printInstBV(const Instruction &Inst) const {
    const BasicBlock *const InstParent = Inst.getParent();
    if (&Inst == &(*InstParent->begin())) {
      MeetOpConstRange MeetOperands = getMeetOperands(*InstParent);
      // If the list of meet operands is empty, then we are at the boundary,
      // hence print the BC.
      if (MeetOperands.begin() == MeetOperands.end()) {
        outs() << "\tBC: ";
        printDomainWithMask(bc());
        outs() << "\n";
      } else {
        outs() << "\tMeetOp: ";
        printDomainWithMask(meet(MeetOperands));
        outs() << "\n";
      }
    } // if (&inst == &(*InstParent->begin()))
    outs() << Inst << "\n";
    outs() << "\t";
    printDomainWithMask(InstDomainMap.at(&Inst));
    outs() << "\n";
  }
  /**
   * @brief Dump, ∀inst ∈ @c F , the associated domain.
   */
  void printInstDomainMap(const Function &F) const {
    outs() << "**************************************************"
           << "\n"
           << "* Instruction-Domain Mapping"
           << "\n"
           << "**************************************************"
           << "\n";
    for (const auto &Inst : instructions(F)) {
      printInstBV(Inst);
    }
  }
  /*****************************************************************************
   * Meet Operator
   *****************************************************************************/
  METHOD_ENABLE_IF_DIRECTION(Direction::kForward, MeetOpConstRange)
  getMeetOperands(const BasicBlock &BB) const { return predecessors(&BB); }
  /**
   * @brief Apply the meet operator to the operands.
   */
  std::vector<TDomainElemRepr>
  meet(const MeetOpConstRange &MeetOperands) const {
    TMeetOp MeetOp;
    /**
     * @todo(cscd70) Please complete the defintion of this method.
     */

    return std::vector<TDomainElemRepr>(Domain.size());
  }
  /*****************************************************************************
   * Transfer Function
   *****************************************************************************/
  /**
   * @brief  Apply the transfer function at instruction @c Inst to the input
   *         domain values to get the output.
   * @return true if @c OBV has been changed, false otherwise
   *
   * @todo(cscd70) Please implement this method for every child class.
   */
  virtual bool transferFunc(const Instruction &Inst,
                            const std::vector<TDomainElemRepr> &IBV,
                            std::vector<TDomainElemRepr> &OBV) = 0;
  /*****************************************************************************
   * CFG Traversal
   *****************************************************************************/
  /**
   * @brief Boundary Condition
   */
  std::vector<TDomainElemRepr> bc() const {
    return std::vector<TDomainElemRepr>(Domain.size());
  }
  /**
   * @brief  Return the traversal order of the basic blocks.
   */
  METHOD_ENABLE_IF_DIRECTION(Direction::kForward, BBTraversalConstRange)
  getBBTraversalOrder(const Function &F) const {
    return make_range(F.begin(), F.end());
  }
  /**
   * @brief Return the traversal order of the instructions.
   *
   * @todo(cscd70) Please modify the definition (and the above typedef
   *               accordingly) for the optimal traversal order.
   */
  METHOD_ENABLE_IF_DIRECTION(Direction::kForward, InstTraversalConstRange)
  getInstTraversalOrder(const BasicBlock &BB) const {
    return make_range(BB.begin(), BB.end());
  }

protected:
  /**
   * @brief  Traverse through the CFG and update @c inst_bv_map .
   * @return true if changes are made to @c inst_bv_map , false otherwise
   *
   * @todo(cscd70) Please implement this method.
   */
  bool traverseCFG(const Function &F) { return false; }

public:
  virtual ~Framework() {}

protected:
  /**
   * @todo(cscd70) Please implement this method for every child class.
   */
  virtual void initializeDomainFromInst(const Instruction &Inst) = 0;
  /**
   * @brief Initialize the domain from each instruction and/or argument.
   */
  void initializeDomain(const Function &F) {
    for (const auto &Inst : instructions(F)) {
      initializeDomainFromInst(Inst);
    }
  }

public:
  virtual bool runOnFunction(Function &F) {
    // initialize the domain
    initializeDomain(F);
    // apply the initial conditions
    TMeetOp MeetOp;
    for (const auto &Inst : instructions(F)) {
      InstDomainMap.emplace(&Inst, MeetOp.top(Domain.size()));
    }
    // keep traversing until no changes have been made to the
    // instruction-domain mapping
    while (traverseCFG(F)) {
    }
    printInstDomainMap(F);
    return false;
  }
};

} // namespace dfa
