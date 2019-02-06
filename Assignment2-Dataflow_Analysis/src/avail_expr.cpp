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
class AvailExpr : public dfa::Framework < Expression, 
	                                  dfa::Direction::Forward >
{
protected:
	virtual void _initializeDomain(const Function & func) override final
	{
		// @TODO
	}
	virtual void  _applyBoundaryConditions() override final
	{
		// @TODO
	}
	virtual void __applyInitialConditions(const Function & func) override final
	{
		// @TODO
	}
	virtual bool __meetOp(const BasicBlock & bb) override final
	{
		// @TODO
		return false;
	}
	virtual bool __instTransferFunc(const Instruction & inst, 
	                                const BitVector & ibv, BitVector & obv) override final
	{
		// @TODO
		return false;
	}
public:
	static char ID;

	AvailExpr() : dfa::Framework < Expression, 
		                       dfa::Direction::Forward > (ID) {}
};

char AvailExpr::ID = 1; 
RegisterPass < AvailExpr > Y ("avail-expr", "Available Expression");
} // namespace anonymous