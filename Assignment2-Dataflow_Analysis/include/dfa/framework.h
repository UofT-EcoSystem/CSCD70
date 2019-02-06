#pragma once

#include <utility> // std::pair
#include <type_traits>
#include <unordered_set> // Hash Set
#include <unordered_map> // Hash Map

#include <llvm/Pass.h>
#include <llvm/ADT/BitVector.h> // BitVector
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
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
		for (const auto & element : _domain)
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

	// Initialize the Instruction-IO BitVector Pair Mapping.
	virtual void _initializeInstBVMap(const Function & func)
	{
		for (auto & bb : func)
		{
			for (auto & inst : bb)
			{
				_inst_bv_map.insert(std::make_pair(&inst, 
					BitVector(_domain.size())));
			}
		}
		__applyInitialConditions(func);
	}
	// Apply the Boundary Conditions, should be called by 'runOnFunction'.
	virtual void  _applyBoundaryConditions() = 0;
	// Apply the Initial  Conditions, should be called by '_initializeInstBVMap'.
	virtual void __applyInitialConditions(const Function & func) = 0;

	// Dump, for each Instruction in the Function, the associated BitVector.
	void _dumpInstBVMap(const Function & func) const
	{
		outs() << "********************************************" << "\n";
		outs() << "* Instruction-IO BitVector Mapping          " << "\n";
		outs() << "********************************************" << "\n";

		for (const auto & bb : func)
		{
			for (const auto & inst : bb)
			{
				outs() << "Instruction: " << inst << "\n";

				const BitVector & bv = _inst_bv_map.at(&inst);
				
				outs() << "\t";
				__dumpDomainWithMask(bv); 
				outs() << "\n";
			}
		}
	}

	/***********************************************************************
	 * Meet Operator and Transfer Function
	 ***********************************************************************/
	
	virtual bool __meetOp(const BasicBlock & bb) = 0;
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
	
	virtual ~Framework() {}

	// We don't modify the program, so we preserve all analysis.
	virtual void getAnalysisUsage(AnalysisUsage & AU) const
	{
		AU.setPreservesAll();
	}

	virtual bool runOnFunction(Function & func)
	{
		// Initialize the Domain and the Instruction-IO BitVector mapping.
		_initializeDomain   (func);
		_initializeInstBVMap(func);

		bool is_convergent;
		do 
		{
			is_convergent = true;

			_applyBoundaryConditions();

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