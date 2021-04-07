// RUN: clang -O0 -Xclang -disable-O0-optnone -emit-llvm -c %s -o %basename_t.bc
// RUN: opt -S -mem2reg %basename_t.bc -o %basename_t.ll
// RUN: llc -load %dylibdir/libLICM.so -regalloc=basic %basename_t.ll -o %basename_t.s

// @todo(cscd70) Please replace the register allocator with 'intfgraph' after
//               you have completed the interference graph register allocator.

// RUN: clang %basename_t.s -o %basename_t.exe
// RUN: ./%basename_t.exe | FileCheck %s
// CHECK: 023
#include <stdio.h>

int g = 0;

int def() {
  return g++;
}

void use(int i) {
  printf("%d", i);
}

int main() {
  int A = def(), B, C, D;

  if (A) {
    B = def();
    use(A);
    D = B + 1;
  } else {
    C = def();
    use(A);
    D = C + 2;
  }
  A = def();
  use(A);
  use(D);
  return 0;
}
