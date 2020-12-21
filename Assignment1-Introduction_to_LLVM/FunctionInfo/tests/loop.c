// RUN: clang -O2 -emit-llvm -c %s -o - | \
// RUN: opt -load-pass-plugin %dylibdir/libFunctionInfo%dylibext \
// RUN:     -passes=function-info -disable-output 2>&1 - | \
// RUN: FileCheck %s

// CHECK: CSCD70 Functions Information Pass
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
