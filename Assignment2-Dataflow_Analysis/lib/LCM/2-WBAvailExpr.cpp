#include "2-WBAvailExpr.h"

char WBAvailExprWrapperPass::ID = 0;
static RegisterPass<WBAvailExprWrapperPass> X("wb-avail-expr",
                                              "Will-be-Available Expression");
