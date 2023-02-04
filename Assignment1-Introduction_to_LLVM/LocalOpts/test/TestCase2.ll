; RUN: opt -load-pass-plugin=%dylibdir/libLocalOpts.so \
; RUN:     -p=algebraic-identity,strength-reduction,multi-inst-opt \
; RUN:     -S %s -o %basename_t
; RUN: FileCheck --match-full-lines %s --input-file=%basename_t

; #include <stdio.h>

; void foo(int a) {
;   int r0 = a + 0;
;   int r1 = r0 * 16;
;   int r2 = 0, r3 = 0, r4 = 0, r5 = 0;
;   if (a) {
;     r2 = r1 * r0;
;     r3 = r2 / a;
;   } else {
;     r4 = r2 / 10;
;     r5 = 54 * r3;
;   }
;   int r6 = r1 / 16;
;   int r7 = r5 / 54;
;   int r8 = r4 / 1;
;   int r9 = r7 - 0;
;   printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", r0, r1, r2, r3, r4, r5, r6, r7, r8,
;          r9);
; }

@.str = private unnamed_addr constant [31 x i8] c"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local void @foo(i32 noundef %0) #0 {
; CHECK-LABEL: define dso_local void @foo(i32 noundef %0) {
; @todo(CSCD70) Please complete the CHECK directives.
  %2 = add nsw i32 %0, 0
  %3 = mul nsw i32 %2, 16
  %4 = icmp ne i32 %0, 0
  br i1 %4, label %5, label %8

5:                                                ; preds = %1
  %6 = mul nsw i32 %3, %2
  %7 = sdiv i32 %6, %0
  br label %11

8:                                                ; preds = %1
  %9 = sdiv i32 0, 10
  %10 = mul nsw i32 54, 0
  br label %11

11:                                               ; preds = %8, %5
  %.03 = phi i32 [ %6, %5 ], [ 0, %8 ]
  %.02 = phi i32 [ %7, %5 ], [ 0, %8 ]
  %.01 = phi i32 [ 0, %5 ], [ %9, %8 ]
  %.0 = phi i32 [ 0, %5 ], [ %10, %8 ]
  %12 = sdiv i32 %3, 16
  %13 = sdiv i32 %.0, 54
  %14 = sdiv i32 %.01, 1
  %15 = sub nsw i32 %13, 0
  %16 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %2, i32 noundef %3, i32 noundef %.03, i32 noundef %.02, i32 noundef %.01, i32 noundef %.0, i32 noundef %12, i32 noundef %13, i32 noundef %14, i32 noundef %15)
  ret void
}

declare i32 @printf(ptr noundef, ...) #1
