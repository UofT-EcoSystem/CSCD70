; RUN: opt -S -load-pass-plugin=%dylibdir/libDFA.so \
; RUN:     -p=const-prop %s -o %basename_t 2>%basename_t.log
; @todo(CSCD70): FileCheck --match-full-lines %s --input-file=%basename_t.log

; int Loop() {
;   int i = 1, j = 1;
;   for (int k = 0; k < 100;) {
;     if (j < 20) {
;       j = i;
;       k = k + 1;
;     } else {
;       j = k;
;       k = k + 2;
;     }
;   }
;   return j;
; }
; @todo(CSCD70) Please complete the CHECK directives.
define i32 @Loop() {
  br label %1

1:                                                ; preds = %9, %0
  %.01 = phi i32 [ 1, %0 ], [ %.12, %9 ]
  %.0 = phi i32 [ 0, %0 ], [ %.1, %9 ]
  %2 = icmp slt i32 %.0, 100
  br i1 %2, label %3, label %10

3:                                                ; preds = %1
  %4 = icmp slt i32 %.01, 20
  br i1 %4, label %5, label %7

5:                                                ; preds = %3
  %6 = add nsw i32 %.0, 1
  br label %9

7:                                                ; preds = %3
  %8 = add nsw i32 %.0, 2
  br label %9

9:                                                ; preds = %7, %5
  %.12 = phi i32 [ 1, %5 ], [ %.0, %7 ]
  %.1 = phi i32 [ %6, %5 ], [ %8, %7 ]
  br label %1

10:                                               ; preds = %1
  ret i32 %.01
}
