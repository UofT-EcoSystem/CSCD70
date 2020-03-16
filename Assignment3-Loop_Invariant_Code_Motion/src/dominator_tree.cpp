#include "./dominator_tree.h"

#include <llvm/PassSupport.h>

using namespace llvm;


char DominatorTree::ID = 0; 
RegisterPass < DominatorTree > X ("dominators", "dominators");
