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
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;


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

template<typename T>
concept MeetOpConcept = std::is_base_of<MeetOp, T>::value;

template<typename T>
concept BCConcept = std::is_base_of<BC, T>::value;

/**
 * @brief  Dataflow Analysis Framework
 *  
 * @tparam TDomainElement   Domain Element
 * @tparam TDirection       Direction of Analysis
 * @tparam TMeetOp          Meet Operator
 * @tparam TBC              Boundary Condition
 * @tparam TDomainElemRepr  Domain Element Representation (bool by Default)
 */
template<typename TDomainElement, Direction TDirection,
         MeetOpConcept TMeetOp, BCConcept TBC,
         typename TDomainElemRepr = bool>
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
  using typename FrameworkTypeSupport<TDirection>::MeetOpConstRange;
  using typename FrameworkTypeSupport<TDirection>::BBTraversalConstRange;
  using typename FrameworkTypeSupport<TDirection>::InstTraversalConstRange;
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
      } catch (const std::invalid_argument& ia) {}
    }
    for (const auto& Arg : F.args()) {
      try {
        Domain.emplace(Arg);
      } catch (const std::invalid_argument& ia) {}
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
