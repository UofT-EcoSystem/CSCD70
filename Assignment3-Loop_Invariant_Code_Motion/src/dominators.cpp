#include "./dominators.h"

#include <llvm/PassSupport.h>

using namespace llvm;


char Dominators::ID = 0; 
RegisterPass < Dominators > X ("dominators", "dominators");
