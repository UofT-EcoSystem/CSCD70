// clang-format off
// RUN: clang -Xclang -disable-O0-optnone -S -emit-llvm -c %s -o %basename_t.ll
// RUN: opt -p=mem2reg -S -o %basename_t.M2R.ll %basename_t.ll
// RUN: opt -load-pass-plugin=%dylibdir/libSCEV.so -p=some-pass-that-uses-scev \
// RUN:     -disable-output %basename_t.M2R.ll 2>&1 | \
// RUN: FileCheck --match-full-lines --check-prefix=SCEV %s
// RUN: opt -p="print<scalar-evolution>" \
// RUN:     -disable-output %basename_t.M2R.ll 2>&1 | \
// RUN: FileCheck --match-full-lines --check-prefix=SCEVPRINT %s

// clang-format on
#include <stdio.h>

void foo() {
  for (int i = 0; i < 100; ++i) {
    // SCEV: Instruction=  %11 = add nsw i32 %10, 7 : {7,+,6,+,10,+,6}<%1>
    // SCEVPRINT:   -->  {7,+,6,+,10,+,6}<%1> U: full-set S: full-set          Exits: 1020307          LoopDispositions: { %1: Computable }
    int j = i * i * i + 2 * i * i + 3 * i + 7;
    // SCEV: Instruction=  %18 = sub nsw i32 %16, %17 : 1
    // SCEVPRINT:   -->  1 U: [1,2) S: [1,2)           Exits: 1                LoopDispositions: { %1: Invariant }
    int k = (i + 1) * (i + 1) - i * i - 2 * i;
  }
}
