//      Compile the test case into assembly bytecode.
// RUN: clang -O2 -emit-llvm -c %s -o %basename_t.bc
//      Disassemble the bytecode into readable format.
// RUN: llvm-dis %basename_t.bc -o=%basename_t.ll
//      Run the FunctionInfo pass.
// RUN: env LD_LIBRARY_PATH=%dylibdir opt -load libFunctionInfo%dylibext -function-info -disable-output 2>&1 %basename_t.bc | \
//      Check the output "CSCD70 Function Information Pass".
// RUN: FileCheck --match-full-lines %s

// CHECK: CSCD70 Function Information Pass
int g;

int g_incr(int c) {
  g += c;
  return g;
}

int loop(int a, int b, int c) {
  int i, ret = 0;

  for (i = a; i < b; i++) {
    g_incr(c);
  }

  return ret + g;
}
