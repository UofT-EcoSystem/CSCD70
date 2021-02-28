#pragma once // NOLINT(llvm-header-guard)

#include <type_traits>
#include <unordered_map>
#include <vector>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace dfa {

template <typename TDomainElemRepr> //
class MeetOp;

/// Analysis Direction, used as Template Parameter
enum class Direction { kForward, kBackward };

template <Direction TDirection> //
struct FrameworkTypeSupport {};

/**
 * @todo(cscd70) Modify the typedefs if necessary.
 */
template <> //
struct FrameworkTypeSupport<Direction::kForward> {
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

  static_assert(std::is_base_of<MeetOp<TDomainElemRepr>, TMeetOp>::value,
                "TMeetOp has to inherit from MeetOp");

/**
 * @brief Selectively enables methods depending on the analysis direction.
 * @param dir  Direction of Analysis
 * @param ret_type  Return Type
 */
#define METHOD_ENABLE_IF_DIRECTION(dir, ret_type)                              \
  template <Direction _TDirection = TDirection>                                \
  typename std::enable_if_t<_TDirection == dir, ret_type>

protected:
  using DomainVal_t = std::vector<TDomainElemRepr>;

private:
  using MeetOperands_t = std::vector<DomainVal_t>;
  using BBTraversalConstRange =
      typename FrameworkTypeSupport<TDirection>::BBTraversalConstRange;
  using InstTraversalConstRange =
      typename FrameworkTypeSupport<TDirection>::InstTraversalConstRange;

protected:
  // Domain
  std::vector<TDomainElem> Domain;
  // Instruction-Domain Value Mapping
  std::unordered_map<const Instruction *, DomainVal_t> InstDomainValMap;
  /*****************************************************************************
   * Auxiliary Print Subroutines
   *****************************************************************************/
private:
  /**
   * @brief Print the domain with mask. E.g., If domian = {%1, %2, %3,},
   *        dumping it with mask = 001 will give {%3,}.
   */
  void printDomainWithMask(const DomainVal_t &Mask) const {
    errs() << "{";
    assert(Mask.size() == Domain.size() &&
           "The size of mask must be equal to the size of domain.");
    unsigned MaskIdx = 0;
    for (const auto &Elem : Domain) {
      if (!Mask[MaskIdx++]) {
        continue;
      }
      errs() << Elem << ", ";
    } // for (MaskIdx ∈ [0, Mask.size()))
    errs() << "}";
  }
  /**
   * @todo(cscd70) Please provide an instantiation for the backward pass.
   */
  METHOD_ENABLE_IF_DIRECTION(Direction::kForward, void)
  printInstDomainValMap(const Instruction &Inst) const {
    const BasicBlock *const InstParent = Inst.getParent();
    if (&Inst == &(InstParent->front())) {
      errs() << "\t";
      printDomainWithMask(getBoundaryVal(*InstParent));
      errs() << "\n";
    } // if (&Inst == &(*InstParent->begin()))
    outs() << Inst << "\n";
    errs() << "\t";
    printDomainWithMask(InstDomainValMap.at(&Inst));
    errs() << "\n";
  }
  /**
   * @brief Dump, ∀inst ∈ F, the associated domain value.
   */
  void printInstDomainValMap(const Function &F) const {
    // clang-format off
    errs() << "**************************************************" << "\n"
           << "* Instruction-Domain Value Mapping" << "\n"
           << "**************************************************" << "\n";
    // clang-format on
    for (const auto &Inst : instructions(F)) {
      printInstDomainValMap(Inst);
    }
  }
  /*****************************************************************************
   * BasicBlock Boundary
   *****************************************************************************/
protected:
  virtual DomainVal_t getBoundaryVal(const BasicBlock &BB) const {
    MeetOperands_t MeetOperands = getMeetOperands(BB);
    if (MeetOperands.begin() == MeetOperands.end()) {
      // If the list of meet operands is empty, then we are at the boundary,
      // hence obtain the BC.
      return bc();
    }
    return meet(MeetOperands);
  }

private:
  /**
   * @todo(cscd70) Please provide an instantiation for the backward pass.
   */
  METHOD_ENABLE_IF_DIRECTION(Direction::kForward, MeetOperands_t)
  getMeetOperands(const BasicBlock &BB) const {
    MeetOperands_t Operands;
    /**
     * @todo(cscd70) Please complete the definition of this method.
     */

    return Operands;
  }
  /**
   * @brief Boundary Condition
   */
  DomainVal_t bc() const { return DomainVal_t(Domain.size()); }
  /**
   * @brief Apply the meet operator to the operands.
   */
  DomainVal_t meet(const MeetOperands_t &MeetOperands) const {
    /**
     * @todo(cscd70) Please complete the defintion of this method.
     */

    return DomainVal_t(Domain.size());
  }
  /*****************************************************************************
   * Transfer Function
   *****************************************************************************/
protected:
  static bool diff(const DomainVal_t &LHS, const DomainVal_t &RHS) {
    if (LHS.size() != RHS.size()) {
      assert(false && "Size of domain values has to be the same");
    }
    for (size_t Idx = 0; Idx < LHS.size(); ++Idx) {
      if (LHS[Idx] != RHS[Idx]) {
        return true;
      }
    }
    return false;
  }

private:
  /**
   * @brief  Apply the transfer function at instruction @c Inst to the input
   *         domain values to get the output.
   * @return true if @c OV has been changed, false otherwise
   *
   * @todo(cscd70) Please implement this method for every child class.
   */
  virtual bool transferFunc(const Instruction &Inst, const DomainVal_t &IV,
                            DomainVal_t &OV) = 0;
  /*****************************************************************************
   * CFG Traversal
   *****************************************************************************/
  /**
   * @brief Return the traversal order of the basic blocks.
   *
   * @todo(cscd70) Please modify the definition (and the above typedef
   *               accordingly) for the optimal traversal order.
   * @todo(cscd70) Please provide an instantiation for the backward pass.
   */
  METHOD_ENABLE_IF_DIRECTION(Direction::kForward, BBTraversalConstRange)
  getBBTraversalOrder(const Function &F) const {
    return make_range(F.begin(), F.end());
  }
  /**
   * @brief Return the traversal order of the instructions.
   *
   * @todo(cscd70) Please provide an instantiation for the backward pass.
   */
  METHOD_ENABLE_IF_DIRECTION(Direction::kForward, InstTraversalConstRange)
  getInstTraversalOrder(const BasicBlock &BB) const {
    return make_range(BB.begin(), BB.end());
  }
  /**
   * @brief  Traverse through the CFG and update instruction-domain value
   *         mapping.
   * @return true if changes are made to the mapping, false otherwise
   *
   * @todo(cscd70) Please implement this method.
   */
  bool traverseCFG(const Function &F) { return false; }
  /*****************************************************************************
   * Domain Initialization
   *****************************************************************************/
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

protected:
  virtual ~Framework() {}

  bool runOnFunction(const Function &F) {
    // initialize the domain
    initializeDomain(F);
    // apply the initial conditions
    TMeetOp MeetOp;
    for (const auto &Inst : instructions(F)) {
      InstDomainValMap.emplace(&Inst, MeetOp.top(Domain.size()));
    }
    // keep traversing until no changes have been made to the
    // instruction-domain value mapping
    while (traverseCFG(F)) {
    }
    printInstDomainValMap(F);
    return false;
  }
};

} // namespace dfa
