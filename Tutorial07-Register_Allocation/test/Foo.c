// RUN: clang -O0 -Xclang -disable-O0-optnone -emit-llvm -c %s -o %basename_t.bc
// RUN: opt -S -p=mem2reg %basename_t.bc -o %basename_t.ll
//      Run the minimal register allocator on the LLVM IR.
// RUN: llc -load %dylibdir/libRegAlloc.so \
// RUN:     -regalloc=minimal --relocation-model=pic \
// RUN:     %basename_t.ll -o %basename_t.s
//      Compile the generated assembly into executable.
// RUN: clang %basename_t.s -o %basename_t.exe
// RUN: ./%basename_t.exe | FileCheck %s
// CHECK: 023
#include <stdio.h>

int g = 0;

#define DEF() g++
#define USE(v) printf("%d", v)

int main() {
  int A = DEF(), B, C, D;

  if (A) {
    B = DEF();
    USE(A);
    D = B + 1;
  } else {
    C = DEF();
    USE(A);
    D = C + 2;
  }
  A = DEF();
  USE(A);
  USE(D);
  return 0;
}
