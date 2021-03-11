// RUN: clang -O2 -S -emit-llvm -c %s -o %basename_t.bc
// RUN: opt -load %dylibdir/libPassManager.so \
// RUN:     -transform -another-transform -disable-output 2>&1 %basename_t.bc | \
// RUN: FileCheck --match-full-lines %s

// CHECK: Analysis
// CHECK-NEXT: Transform
// CHECK-NEXT: 1, 2, 3, 
// CHECK-NEXT: Another Transform
// CHECK-NEXT: 1, 2, 3, 
int main() {
  return 0;
}
