#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;


namespace {

class Transform final : public ModulePass
{
 private:
  bool runOnBasicBlock(BasicBlock &B) {
    // get the first and second instruction
    Instruction &_1st_inst = *B.begin(),
                &_2nd_inst = *(++B.begin());

    // The address of the 1st instruction is equal to that of the 1st operand of
    // the 2nd instruction
    assert(&_1st_inst == _2nd_inst.getOperand(0));

    // print the 1st instruction
    outs() << _1st_inst << "\n";
    // print the 1st instruction as an operand
    _1st_inst.printAsOperand(outs(), false); outs() << "\n";

    // User-Use-Value
    for (auto *Iter  = _1st_inst.op_begin();
               Iter != _1st_inst.op_end(); ++Iter) {
      Value *Operand = *Iter;

      if (Argument *Arg = dyn_cast<Argument>(Operand)) {
        outs() << "I am function " << Arg->getParent()->getName()
               << "\'s #" << Arg->getArgNo() << " argument" << "\n";
      }
      if (ConstantInt *C = dyn_cast<ConstantInt>(Operand)) {
        outs() << "I am a constant integer of value "
               << C->getValue() << "\n";
      }
    }

    for (auto Iter  = _1st_inst.user_begin(); 
              Iter != _1st_inst.user_end(); ++Iter) {
      outs() << *(dyn_cast<Instruction>(*Iter)) << "\n";
    }
    
    for (auto Iter  = _1st_inst. use_begin();
              Iter != _1st_inst. use_end(); ++Iter) {
      outs() << *(dyn_cast<Instruction>(Iter->getUser())) << "\n";
    }

    // Instruction Manipulation
    Instruction *NewInst = BinaryOperator::Create(
        Instruction::Add,
        _1st_inst.getOperand(0),
        _1st_inst.getOperand(0));

    NewInst->insertAfter(&_1st_inst);
    // Q: Is there any alternative to updating each reference separately?
    //    Please check the documentation and try answering this.
    // Q: What happens if we update the use references WITHOUT the insertion?
    _1st_inst.user_begin()->setOperand(0, NewInst);

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
  virtual ~Transform() override {}

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
RegisterPass < Transform > X(
  "transform",
  "Example Transform Pass"); 

}  // anonymous namespace
