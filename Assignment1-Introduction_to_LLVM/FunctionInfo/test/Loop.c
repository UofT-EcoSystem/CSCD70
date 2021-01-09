//      Compile the test case into assembly bytecode.
// RUN: clang -O2 -emit-llvm -c %s -o %basename_t.bc
//      Disassemble the bytecode into readable format. The output Loop.c.ll will
//      be stored in the build/test folder.
// RUN: llvm-dis %basename_t.bc -o=%basename_t.ll
//      Run the FunctionInfo pass. The `-disable-output` option disables
//      outputing the bytecode because we are only checking the pass outputs here.
// RUN: env LD_LIBRARY_PATH=%dylibdir opt -load libFunctionInfo%dylibext -function-info -disable-output 2>&1 %basename_t.bc | \
//      Check the output "CSCD70 Function Information Pass".
// RUN: FileCheck --match-full-lines --check-prefix=SAMPLE %s
/*! @todo(cscd70) Remove the `--check-prefix=SAMPLE` option.
 */

// SAMPLE: CSCD70 Function Information Pass

int g;

// CHECK-LABEL: Function Name: g_incr
// CHECK-NEXT: Number of Arguments: 1
// CHECK-NEXT: Number of Calls: 0
// CHECK-NEXT: Number OF BBs: 1
// CHECK-NEXT: Number of Instructions: 4
int g_incr(int c) {
  g += c;
  return g;
}

// CHECK-LABEL: Function Name: loop
// CHECK-NEXT: Number of Arguments: 3
// CHECK-NEXT: Number of Calls: 0
// CHECK-NEXT: Number OF BBs: 3
// CHECK-NEXT: Number of Instructions: 10
int loop(int a, int b, int c) {
  int i, ret = 0;

  for (i = a; i < b; i++) {
    g_incr(c);
  }

  return ret + g;
}
