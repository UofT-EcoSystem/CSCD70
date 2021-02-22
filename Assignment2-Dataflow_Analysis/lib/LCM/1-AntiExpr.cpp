#include "1-AntiExpr.h"

char AntiExprWrapperPass::ID = 0;
static RegisterPass<AntiExprWrapperPass> X("anti-expr",
                                           "Anticipated Expression");
