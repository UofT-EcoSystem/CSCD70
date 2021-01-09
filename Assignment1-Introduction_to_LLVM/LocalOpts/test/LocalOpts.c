// RUN: clang -O0 -Xclang -disable-O0-optnone -emit-llvm -c %s -o %basename_t.bc
// RUN: 
// RUN: llvm-dis %basename_t.bc -o=%basename_t.ll
// RUN: opt -load %dylibdir/libLocalOpts%dylibext \
// RUN:     -algebraic-identity -strength-reduction -multi-inst-opt \
// RUN:     %basename_t.bc | \
// RUN: FileCheck --match-full-lines %s

int LocalOpts(int a) {
  int r0 = a + 0;
  int r1 = r0 * 16;
  int r2 = r1 * r0;
  int r3 = r2 / a;
  int r4 = r2 / 10;
  int r5 = 54 * r3;
  int r6 = r4 / 128;
  int r7 = r5 / 54;
  int r8 = r4 / 1;
  int r9 = r7 - 0;
  return r9;
}
