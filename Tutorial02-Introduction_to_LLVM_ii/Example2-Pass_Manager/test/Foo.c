// clang-format off
// RUN: clang -O2 -S -emit-llvm -c %s -o %basename_t.bc
// RUN: opt -load-pass-plugin=%dylibdir/libPassManager.so \
// RUN:     -p=some-transform-that-preserves,another-transform,some-transform-that-does-NOT-preserve,another-transform \
// RUN:     -disable-output 2>&1 %basename_t.bc | \
// RUN: FileCheck --match-full-lines %s
// clang-format on

// CHECK: Some Transform that preserves
// CHECK-NEXT: Some Analysis
// CHECK-NEXT: 1, 2, 3,
// CHECK-NEXT: Verifying the validity of the analysis results: Y
// CHECK-NEXT: Another Transform
// CHECK-NEXT: 1, 2, 3,
// CHECK-NEXT: Verifying the validity of the analysis results: N

// CHECK-NEXT: Some Transform that does NOT preserve
// CHECK-NEXT: Some Analysis
// CHECK-NEXT: 1, 2, 3,
// CHECK-NEXT: Verifying the validity of the analysis results: N
// CHECK-NEXT: Another Transform

// Note that `SomeAnalysis` is invoked again, due to the fact that the previous
// transform abandons its results.

// CHECK-NEXT: Some Analysis
// CHECK-NEXT: 1, 2, 3,
// CHECK-NEXT: Verifying the validity of the analysis results: N
int main() { return 0; }
