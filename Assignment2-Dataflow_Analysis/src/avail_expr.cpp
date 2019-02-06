#include "dfa/framework.h"

class Expression
{
private:
	unsigned _opcode; const Value * _lhs, * _rhs;
public:
	Expression(unsigned opcode = 0, 
		   const Value * lhs = nullptr, 
		   const Value * rhs = nullptr) : 
		_opcode(opcode), _lhs(lhs), _rhs(rhs) {}

	bool operator==(const Expression & Expr) const
	{
		// @TODO
		return false;
	}

	unsigned getOpcode() const { return _opcode; }
	const Value * getLHSOperand() const { return _lhs; }
	const Value * getRHSOperand() const { return _rhs; }

	friend raw_ostream & operator<<(raw_ostream & outs, const Expression & Expr);
}; 

raw_ostream & operator<<(raw_ostream & outs, const Expression & Expr)
{
	outs << "[" << Instruction::getOpcodeName(Expr._opcode) << " ";
		Expr._lhs->printAsOperand(outs, false); outs << ", ";
		Expr._rhs->printAsOperand(outs, false);	outs << "]";

	return outs;
}

namespace std {
// Construct a hash code for 'Expression'.
template <>
struct hash < Expression >
{
	std::size_t operator()(const Expression & expr) const
	{
		std::hash < unsigned > unsigned_hasher; std::hash < std::size_t > std_size_t_hasher;

		std::size_t opcode_hash = unsigned_hasher(expr.getOpcode());
		std::size_t lhs_operand_hash = std_size_t_hasher(std::size_t(expr.getLHSOperand()));
		std::size_t rhs_operand_hash = std_size_t_hasher(std::size_t(expr.getRHSOperand()));

		return opcode_hash ^ (lhs_operand_hash << 1) ^ (rhs_operand_hash << 1);
	}
};
} // namespace std

namespace {
class AvailExprMeetOp : public dfa::MeetOpABC
{
	virtual void operator()(const BitVector & parent_bv,
	                              BitVector &  child_bv) const override final
	{
		// @TODO
	}
};

class AvailExprInstTransferFunc : public dfa::InstTransferFuncABC
{
	virtual bool operator()(const Instruction & bb) const override final
	{
		// @TODO
		return false;
	}
};

class AvailExpr : public dfa::Framework < Expression, 
	                                  dfa::Direction::Forward, 
			                  AvailExprMeetOp,
				          AvailExprInstTransferFunc >
{
protected:
	// Initialize the domain.
	virtual void _initializeDomain(const Function & func) override final
	{
		// @TODO
	}
	// Initialize the Instruction-IO BitVector Pair mapping.
	virtual void __applyInitialConditions(const Function & func) override final
	{
		// @TODO
	}
public:
	static char ID;

	AvailExpr() : dfa::Framework < Expression, 
		                       dfa::Direction::Forward, 
		                       AvailExprMeetOp,
				       AvailExprInstTransferFunc > (ID) {}
};

char AvailExpr::ID = 1; 
RegisterPass < AvailExpr > Y ("avail-expr", "Available Expression");
} // namespace anonymous