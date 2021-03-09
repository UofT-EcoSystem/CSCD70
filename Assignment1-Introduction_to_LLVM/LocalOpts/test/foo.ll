; RUN: opt -S -load %dylibdir/libLocalOpts.so \
; RUN:     -algebraic-identity -strength-reduction -multi-inst-opt \
; RUN:     %s -o %basename_t
; RUN: FileCheck --match-full-lines %s --input-file=%basename_t

define i32 @foo(i32 %0) {
; CHECK-LABEL: define i32 @foo(i32 %0) {
; @todo(cscd70) Please complete the CHECK directives.
  %2 = add nsw i32 %0, 0
  %3 = mul nsw i32 %2, 16
  %4 = mul nsw i32 %3, %2
  %5 = sdiv i32 %4, %0
  %6 = sdiv i32 %4, 10
  %7 = mul nsw i32 54, %5
  %8 = sdiv i32 %6, 128
  %9 = sdiv i32 %7, 54
  %10 = sdiv i32 %6, 1
  %11 = sub nsw i32 %9, 0
  ret i32 %11
}
