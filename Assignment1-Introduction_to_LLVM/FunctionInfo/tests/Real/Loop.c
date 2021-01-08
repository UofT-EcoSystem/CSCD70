// RUN: clang -O2 -emit-llvm -c %s -o %basename_t.bc
// RUN: llvm-dis %basename_t.bc -o=%basename_t.ll
// RUN: env LD_LIBRARY_PATH=%dylibdir opt -load libFunctionInfo%dylibext -function-info -disable-output 2>&1 %basename_t.bc | \
// RUN: FileCheck --match-full-lines %s

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
