; RUN: opt -S -load-pass-plugin=%dylibdir/libDFA.so \
; RUN:     -p=lcm %s -o %basename_t 2>%basename_t.log
; @todo(CSCD70): FileCheck --match-full-lines %s --input-file=%basename_t.log

; #include "stdio.h"

; int foo(int a, int b, int c) {
;   if (a > 5) {
;     int g = b + c;
;     printf("%d", g);
;   } else {
;     while (b < 5) {
;       b = b + 1;
;       int d = b + c;
;       printf("%d", d);
;     }
;   }
;   int e = b + c;
;   return e;
; }
; @todo(CSCD70) Please complete the CHECK directives. Note that:
;               - The critical edges have to be broken first.
;               - The outputs from both the 4 analysis passes and the 2
;                 placements have to be checked.
@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1

define i32 @foo(i32 noundef %0, i32 noundef %1, i32 noundef %2) #0 {
  %4 = icmp sgt i32 %0, 5
  br i1 %4, label %5, label %8

5:                                                ; preds = %3
  %6 = add nsw i32 %1, %2
  %7 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %6)
  br label %16

8:                                                ; preds = %3
  br label %9

9:                                                ; preds = %11, %8
  %.0 = phi i32 [ %1, %8 ], [ %12, %11 ]
  %10 = icmp slt i32 %.0, 5
  br i1 %10, label %11, label %15

11:                                               ; preds = %9
  %12 = add nsw i32 %.0, 1
  %13 = add nsw i32 %12, %2
  %14 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %13)
  br label %9

15:                                               ; preds = %9
  br label %16

16:                                               ; preds = %15, %5
  %.1 = phi i32 [ %1, %5 ], [ %.0, %15 ]
  %17 = add nsw i32 %.1, %2
  ret i32 %17
}

declare i32 @printf(ptr noundef, ...) #1
