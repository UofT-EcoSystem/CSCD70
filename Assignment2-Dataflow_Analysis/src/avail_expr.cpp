#include "dfa/framework.h"

namespace {

class Expression
{
private:
        unsigned _opcode; const Value * _lhs, * _rhs;
public:
        Expression(const Instruction & inst)
        {
                // @TODO
        }

        bool operator==(const Expression & Expr) const
        {
                // @TODO
                return false;
        }

        unsigned getOpcode() const { return _opcode; }
        const Value * getLHSOperand() const { return _lhs; }
        const Value * getRHSOperand() const { return _rhs; }

        friend raw_ostream & operator<<(raw_ostream & outs, const Expression & expr);
}; 

raw_ostream & operator<<(raw_ostream & outs, const Expression & expr)
{
        outs << "[" << Instruction::getOpcodeName(expr._opcode) << " ";
        expr._lhs->printAsOperand(outs, false); outs << ", ";
        expr._rhs->printAsOperand(outs, false); outs << "]";

        return outs;
}

}  // namespace anonymous

namespace std {

// Construct a hash code for 'Expression'.
template <>
struct hash < Expression >
{
        std::size_t operator()(const Expression & expr) const
        {
                std::hash < unsigned > unsigned_hasher; std::hash < const Value * > pvalue_hasher;

                std::size_t opcode_hash = unsigned_hasher(expr.getOpcode());
                std::size_t lhs_operand_hash = pvalue_hasher((expr.getLHSOperand()));
                std::size_t rhs_operand_hash = pvalue_hasher((expr.getRHSOperand()));

                return opcode_hash ^ (lhs_operand_hash << 1) ^ (rhs_operand_hash << 1);
        }
};

}  // namespace std

namespace {

class AvailExpr final : public dfa::Framework < Expression, 
                                                dfa::Direction::Forward >
{
protected:
        virtual BitVector IC() const override
        {
                // @TODO
                return BitVector(_domain.size());
        }
        virtual BitVector BC() const override
        {
                // @TODO
                return BitVector(_domain.size());
        }
        virtual BitVector MeetOp(const meetop_const_range & meet_operands) const override
        {
                // @TODO
                return BitVector(_domain.size());
        }
        virtual bool TransferFunc(const Instruction & inst,
                                  const BitVector & ibv,
                                  BitVector & obv) override
        {
                // @TODO
                return false;
        }
        virtual void InitializeDomainFromInstruction(const Instruction & inst) override
        {
                // try to insert the instruction into the domain, and throw an
                // `invalid_argument` exception if failed
                try {
                        _domain.emplace(inst);
                } catch (const std::invalid_argument & ia_except) {}
        }
public:
        static char ID;

        AvailExpr() : dfa::Framework < domain_element_t, 
                                       direction_c > (ID) {}
        virtual ~AvailExpr() override {}
};

char AvailExpr::ID = 1; 
RegisterPass < AvailExpr > Y ("avail_expr", "Available Expression");

} // namespace anonymous
