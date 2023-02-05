#pragma once // NOLINT(llvm-header-guard)

#include "Framework.h"

namespace dfa {

/// @todo(CSCD70) Please modify the traversal ranges.

typedef llvm::iterator_range<llvm::const_pred_iterator>
    ForwardMeetBBConstRange_t;
typedef llvm::iterator_range<llvm::Function::const_iterator>
    ForwardBBConstRange_t;
typedef llvm::iterator_range<llvm::BasicBlock::const_iterator>
    ForwardInstConstRange_t;

template <typename TDomainElem, typename TValue, typename TMeetOp>
class ForwardAnalysis
    : public Framework<TDomainElem, TValue, TMeetOp, ForwardMeetBBConstRange_t,
                       ForwardBBConstRange_t, ForwardInstConstRange_t> {
protected:
  using Framework_t =
      Framework<TDomainElem, TValue, TMeetOp, ForwardMeetBBConstRange_t,
                ForwardBBConstRange_t, ForwardInstConstRange_t>;
  using typename Framework_t::AnalysisResult_t;
  using typename Framework_t::BBConstRange_t;
  using typename Framework_t::InstConstRange_t;
  using typename Framework_t::MeetBBConstRange_t;

  using Framework_t::BVs;
  using Framework_t::DomainIdMap;
  using Framework_t::DomainVector;
  using Framework_t::InstDomainValMap;

  using Framework_t::getName;
  using Framework_t::run;
  using Framework_t::stringifyDomainWithMask;

  void printInstDomainValMap(const llvm::Instruction &Inst) const final {
    using llvm::errs;
    using llvm::outs;
    const llvm::BasicBlock *const ParentBB = Inst.getParent();

    if (&Inst == &(ParentBB->front())) {
      errs() << "\n";
      LOG_ANALYSIS_INFO << "\t" << stringifyDomainWithMask(BVs.at(ParentBB));
    } // if (&Inst == &(*ParentBB->begin()))
    outs() << Inst << "\n";
    LOG_ANALYSIS_INFO << "\t"
                      << stringifyDomainWithMask(InstDomainValMap.at(&Inst));
  }

  MeetBBConstRange_t
  getMeetBBConstRange(const llvm::BasicBlock &BB) const final {
    return llvm::predecessors(&BB);
  }
  InstConstRange_t getInstConstRange(const llvm::BasicBlock &BB) const final {
    return make_range(BB.begin(), BB.end());
  }
  BBConstRange_t getBBConstRange(const llvm::Function &F) const final {
    return make_range(F.begin(), F.end());
  }
};

} // namespace dfa
