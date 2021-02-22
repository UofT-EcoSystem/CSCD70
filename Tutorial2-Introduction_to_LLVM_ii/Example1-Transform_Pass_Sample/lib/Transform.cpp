#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace {

class Transform final : public ModulePass {
private:
  bool runOnBasicBlock(BasicBlock &B) {
    // get the first and second instruction
    Instruction &Inst1st = *B.begin(), &Inst2nd = *(++B.begin());

    // The address of the 1st instruction is equal to that of the 1st operand of
    // the 2nd instruction
    assert(&Inst1st == Inst2nd.getOperand(0));

    // print the 1st instruction
    outs() << Inst1st << "\n";
    // print the 1st instruction as an operand
    Inst1st.printAsOperand(outs(), false);
    outs() << "\n";

    // User-Use-Value
    for (auto *Iter = Inst1st.op_begin(); Iter != Inst1st.op_end(); ++Iter) {
      Value *Operand = *Iter;

      if (Argument *Arg = dyn_cast<Argument>(Operand)) {
        outs() << "I am function " << Arg->getParent()->getName() << "\'s #"
               << Arg->getArgNo() << " argument"
               << "\n";
      }
      if (ConstantInt *C = dyn_cast<ConstantInt>(Operand)) {
        outs() << "I am a constant integer of value " << C->getValue() << "\n";
      }
    }

    for (auto Iter = Inst1st.user_begin(); Iter != Inst1st.user_end(); ++Iter) {
      outs() << *(dyn_cast<Instruction>(*Iter)) << "\n";
    }

    for (auto Iter = Inst1st.use_begin(); Iter != Inst1st.use_end(); ++Iter) {
      outs() << *(dyn_cast<Instruction>(Iter->getUser())) << "\n";
    }

    // Instruction Manipulation
    Instruction *NewInst = BinaryOperator::Create(
        Instruction::Add, Inst1st.getOperand(0), Inst1st.getOperand(0));

    NewInst->insertAfter(&Inst1st);
    // Q: Is there any alternative to updating each reference separately?
    //    Please check the documentation and try answering this.
    // Q: What happens if we update the use references WITHOUT the insertion?
    Inst1st.user_begin()->setOperand(0, NewInst);

    return true;
  }

  bool runOnFunction(Function &F) {
    bool Transformed = false;

    for (auto Iter = F.begin(); Iter != F.end(); ++Iter) {
      if (runOnBasicBlock(*Iter)) {
        Transformed = true;
      }
    }

    return Transformed;
  }

public:
  static char ID;

  Transform() : ModulePass(ID) {}

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
  }

  virtual bool runOnModule(Module &M) override {
    bool Transformed = false;

    for (auto Iter = M.begin(); Iter != M.end(); ++Iter) {
      if (runOnFunction(*Iter)) {
        Transformed = true;
      }
    }
    return Transformed;
  }
};

char Transform::ID = 0;
RegisterPass<Transform> X("transform", "Example Transform Pass");

} // anonymous namespace
