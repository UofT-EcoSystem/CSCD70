
// RUN: 
// RUN: llc -load %dylibdir/libRegAlloc%dylibext \
// RUN:     -regalloc=cscd70 -march=mips %basename_t.ll -o %basename_t.s
// RUN: FileCheck --match-full-lines %s --input-file=%basename_t.s

int sum(int a, int b) {
  int res = 1;
  for (int i = a; i < b; i++) {
    res += i;
  }
  return res;
}
