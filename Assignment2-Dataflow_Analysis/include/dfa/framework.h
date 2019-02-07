#pragma once

#include <type_traits>
#include <unordered_set>
#include <unordered_map>

#include <llvm/Pass.h>
#include <llvm/ADT/BitVector.h>
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

#define LLVM_ASSERT_W_MSG(assert_cond, msg) 					\
{										\
	if (!(assert_cond))							\
	{									\
		errs() << "[" << __FILE__ << " +" << __LINE__ << "] " <<	\
			"Assertion \"" << #assert_cond << "\" " << 		\
			"failed with Error Message: \"" << msg << "\".\n";	\
		exit(EXIT_FAILURE);						\
	}									\
}

namespace dfa {
enum class Direction { Forward, Backward };

template < typename TDomainElement, Direction TDirection >
class Framework : public FunctionPass
{
protected:

	/***********************************************************************
	 * Domain
	 ***********************************************************************/

	std::unordered_set < TDomainElement > _domain; // Domain

	// Initialize the domain.
	virtual void _initializeDomain(const Function & func) = 0;

	// Dump the domain with mask, used for debugging purpose.
	// E.g., If your domain is {%1, %2, %3}, 
	//       dumping it with mask '001' will give you {%3, }.
	void __dumpDomainWithMask(const BitVector & mask) const
	{
		LLVM_ASSERT_W_MSG(_domain.size() == mask.size(), 
			_domain.size() << " != " << mask.size());
		
		outs() << "{";

		unsigned mask_idx = 0;
		for (auto & element : _domain)
		{
			if (!mask[mask_idx++])
			{
				continue;
			}
			outs() << element << ", ";
		}
		outs() << "}";
	}

	/***********************************************************************
	 * Instruction-BitVector Mapping
	 ***********************************************************************/

	// Mapping from Instruction Pointer to BitVector 
	std::unordered_map < const Instruction *, BitVector > _inst_bv_map;

	virtual BitVector __getBoundaryCondition(const Function & func,
	                                         const BasicBlock & bb) const = 0;
	// Initialize the Instruction-BitVector Mapping.
	virtual void _initializeInstBVMap(const Function & func) = 0;

	// Dump, for each Instruction in the Function, the associated BitVector.
	void _dumpInstBVMap(const Function & func) const
	{
		outs() << "********************************************" << "\n";
		outs() << "* Instruction-BitVector Mapping             " << "\n";
		outs() << "********************************************" << "\n";

		for (auto & bb : func)
		{
			for (auto & inst : bb)
			{
				const BitVector & bv = _inst_bv_map.at(&inst);
					
				if (TDirection == Direction::Forward)
				{
					if (&bb   == &(func.getEntryBlock()) && 
					    &inst == &(*bb.begin()))
					{
						outs() << "Boundary Condition: ";
						__dumpDomainWithMask(__getBoundaryCondition(func, bb));
						outs() << "\n";
					}

					outs() << "Instruction: " << inst << "\n";
					outs() << "\t"; __dumpDomainWithMask(bv); outs() << "\n";
				}
				else if (TDirection == Direction::Backward)
				{
					outs() << "\t"; __dumpDomainWithMask(bv); outs() << "\n";
					outs() << "Instruction: " << inst << "\n";

					if (isa < ReturnInst > (inst) || 
					    isa < UnreachableInst > (inst))
					{
						outs() << "Boundary Condition: ";
						__dumpDomainWithMask(__getBoundaryCondition(func, bb));
						outs() << "\n";
					}
				}
			}
		}
	}

	/***********************************************************************
	 * Meet Operator and Transfer Function
	 ***********************************************************************/
	
	virtual BitVector __meetOp(const BasicBlock & bb) = 0;
	virtual bool __instTransferFunc(const Instruction & inst, 
	                                const BitVector & ibv, BitVector & obv) = 0;

	/***********************************************************************
	 * CFG Traversal
	 ***********************************************************************/

	template < Direction _TDirection = TDirection >
	std::enable_if_t < _TDirection == Direction:: Forward, bool >
	_traverseCFG(const Function & func)
	{
		// @TODO
		return false;
	}
	template < Direction _TDirection = TDirection >
	std::enable_if_t < _TDirection == Direction::Backward, bool >
	_traverseCFG(const Function & func)
	{
		// @TODO
		return false;
	}

public:
	Framework(char ID) : FunctionPass(ID) {}

	// We don't modify the program, so we preserve all analysis.
	virtual void getAnalysisUsage(AnalysisUsage & AU) const
	{
		AU.setPreservesAll();
	}

	virtual bool runOnFunction(Function & func)
	{
		// Initialize the Domain and the Instruction-BitVector mapping.
		_initializeDomain   (func);
		_initializeInstBVMap(func);

		bool is_convergent;
		do 
		{
			is_convergent = true;

			if (_traverseCFG(func))
			{
				is_convergent = false;
			}
		} while (!is_convergent);

		_dumpInstBVMap(func);

		return false;
	}
};
} // namespace dfa
