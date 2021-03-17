; RUN: opt -S -load %dylibdir/libLICM.so \
; RUN:     -loop-invariant-code-motion %s -o %basename_t
; RUN: FileCheck --match-full-lines %s

; #include <stdio.h>

; void print(int a, int h, int m, int n,
;            int q, int r, int y, int z) {
;   printf("%d,%d,%d,%d,%d,%d,%d,%d\n", a, h, m, n, q, r, y, z);
; }

; void foo() {
;   int a = 9, h, m, n, q, r, y = 0, z = 4;

; LOOP:
;   z = z + 1;
;   y = 5;
;   q = 7;
;   if (z < 50) {
;     a = a + 2;
;     h = 3;
;   } else {
;     a = a - 1;
;     h = 4;
;     if (z >= 100) {
;       goto EXIT;
;     }
;   }
;   m = y + 7;
;   n = h + 2;
;   y = 7;
;   r = q + 5;
;   goto LOOP;
; EXIT:
;   print(a, h, m, n, q, r, y, z);
; }
@.str = private constant [25 x i8] c"%d,%d,%d,%d,%d,%d,%d,%d\0A\00", align 1

define void @print(i32 %0, i32 %1, i32 %2, i32 %3, i32 %4, i32 %5, i32 %6, i32 %7) {
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @.str, i64 0, i64 0), i32 %0, i32 %1, i32 %2, i32 %3, i32 %4, i32 %5, i32 %6, i32 %7)
  ret void
}

declare i32 @printf(i8*, ...)

define void @foo() {
  br label %1

1:                                                ; preds = %9, %0
  %.05 = phi i32 [ undef, %0 ], [ %12, %9 ]
  %.04 = phi i32 [ 4, %0 ], [ %2, %9 ]
  %.03 = phi i32 [ undef, %0 ], [ %11, %9 ]
  %.02 = phi i32 [ undef, %0 ], [ %10, %9 ]
  %.0 = phi i32 [ 9, %0 ], [ %.1, %9 ]
  %2 = add nsw i32 %.04, 1
  %3 = icmp slt i32 %2, 50
  br i1 %3, label %4, label %6

4:                                                ; preds = %1
  %5 = add nsw i32 %.0, 2
  br label %9

6:                                                ; preds = %1
  %7 = sub nsw i32 %.0, 1
  %8 = icmp sge i32 %2, 100
  br i1 %8, label %13, label %9

9:                                                ; preds = %6, %4
  %.01 = phi i32 [ 3, %4 ], [ 4, %6 ]
  %.1 = phi i32 [ %5, %4 ], [ %7, %6 ]
  %10 = add nsw i32 5, 7
  %11 = add nsw i32 %.01, 2
  %12 = add nsw i32 7, 5
  br label %1

13:                                               ; preds = %6
  call void @print(i32 %7, i32 4, i32 %.02, i32 %.03, i32 7, i32 %.05, i32 5, i32 %2)
  ret void
}
