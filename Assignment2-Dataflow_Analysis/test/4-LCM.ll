; RUN: opt -S -load-pass-plugin=%dylibdir/libDFA.so \
; RUN:     -p=lcm %s -o %basename_t 2>%basename_t.log
; @todo(CSCD70): FileCheck --match-full-lines %s --input-file=%basename_t.log

; int foo(int a, int b, int c) {
;   if (a > 5) {
;     int g = b + c;
;   } else {
;     while (b < 5) {
;       b = b + 1;
;       int d = b + c;
;     }
;   }
;   int e = b + c;
;   return e;
; }
; @todo(CSCD70) Please complete the CHECK directives. Note that:
;               - The critical edges have to be broken first.
;               - The outputs from both the 4 analysis passes and the 2
;                 placements have to be checked.
define i32 @foo(i32 noundef %0, i32 noundef %1, i32 noundef %2) {
  %4 = icmp sgt i32 %0, 5
  br i1 %4, label %5, label %7

5:                                                ; preds = %3
  %6 = add nsw i32 %1, %2
  br label %14

7:                                                ; preds = %3
  br label %8

8:                                                ; preds = %10, %7
  %.0 = phi i32 [ %1, %7 ], [ %11, %10 ]
  %9 = icmp slt i32 %.0, 5
  br i1 %9, label %10, label %13

10:                                               ; preds = %8
  %11 = add nsw i32 %.0, 1
  %12 = add nsw i32 %11, %2
  br label %8

13:                                               ; preds = %8
  br label %14

14:                                               ; preds = %13, %5
  %.1 = phi i32 [ %1, %5 ], [ %.0, %13 ]
  %15 = add nsw i32 %.1, %2
  ret i32 %15
}
