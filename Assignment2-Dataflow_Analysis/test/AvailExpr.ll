; RUN: opt -S -load %dylibdir/libDFA.so \
; RUN:     -avail-expr %s -o %basename_t 2>%basename_t.log
; RUN: FileCheck --match-full-lines %s --input-file=%basename_t.log

; int main(int argc, char *argv[]) {
;   int a, b, c, d, e, f;
;   a = 50;
;   b = argc + a;
;   c = 96;
;   e = b + c;
;   if (a < b) {
;     f = b - a;
;     e = c * b;
;   } else {
;     f = b + a;
;     e = c * b;
;   }
;   b = a - c;
;   d = b + f;
;   return 0;
; }
; CHECK-LABEL: **************************************************
; CHECK-NEXT:  * Instruction-Domain Value Mapping
; CHECK-NEXT:  **************************************************
; @todo(cscd70) Please complete the CHECK directives.
define i32 @main(i32 %0, i8** %1) {

  %3 = add nsw i32 %0, 50
  %4 = add nsw i32 %3, 96
  %5 = icmp slt i32 50, %3
  br i1 %5, label %6, label %9

6:                                                ; preds = %2
  %7 = sub nsw i32 %3, 50
  %8 = mul nsw i32 96, %3
  br label %12

9:                                                ; preds = %2
  %10 = add nsw i32 %3, 50
  %11 = mul nsw i32 96, %3
  br label %12

12:                                               ; preds = %9, %6
  %.0 = phi i32 [ %7, %6 ], [ %10, %9 ]
  %13 = sub nsw i32 50, 96
  %14 = add nsw i32 %13, %.0
  ret i32 0
}
