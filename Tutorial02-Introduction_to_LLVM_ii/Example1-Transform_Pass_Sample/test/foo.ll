; RUN: opt -S -load %dylibdir/libTransformPassSample.so \
; RUN:     -transform %s -o %basename_t
; RUN: FileCheck --match-full-lines %s --input-file=%basename_t

; int foo(int e, int a) {
;   int b = a + 1;
;   int c = b * 2;
;   b = e << 1;
;   int d = b / 4;
;   return c * d;
; }
; CHECK-LABEL: define i32 @foo(i32 %0, i32 %1) {
define i32 @foo(i32 %0, i32 %1) {
  ; CHECK-NEXT: %3 = add nsw i32 %1, 1
  ; CHECK-NEXT: %4 = add i32 %1, %1
  %3 = add nsw i32 %1, 1
  ; CHECK-NEXT: %5 = mul nsw i32 %4, 2
  %4 = mul nsw i32 %3, 2
  ; @note The same variable 'b' gets assigned to a different value '%5'.
  ; CHECK-NEXT: %6 = shl i32 %0, 1
  %5 = shl i32 %0, 1
  ; CHECK-NEXT: %7 = sdiv i32 %6, 4
  %6 = sdiv i32 %5, 4
  ; CHECK-NEXT: %8 = mul nsw i32 %5, %7
  %7 = mul nsw i32 %4, %6
  ; CHECK-NEXT: ret i32 %8
  ret i32 %7
}
