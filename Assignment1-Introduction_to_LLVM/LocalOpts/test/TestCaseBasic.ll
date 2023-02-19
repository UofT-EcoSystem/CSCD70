; RUN: opt -load-pass-plugin=%dylibdir/libLocalOpts.so \
; RUN:     -p=algebraic-identity,strength-reduction,multi-inst-opt \
; RUN:     -S %s -o %basename_t
; RUN: FileCheck --match-full-lines %s --input-file=%basename_t

; #include <stdio.h>

; void AlgebraicIdentity(int a) {
;   int r0 = a + 0;
;   int r1 = 0 + a;
;   int r2 = a * 1;
;   int r3 = 1 * a;
;   int r4 = a - 0;
;   int r5 = a / 1;
;   printf("%d,%d,%d,%d,%d,%d\n", r0, r1, r2, r3, r4, r5);
; }

; void StrengthReduction(int a) {
;   int r0 = a * 2;
;   int r1 = 64 * a;
;   int r2 = a / 4;
;   int r3 = a / 128;
;   printf("%d,%d,%d,%d\n", r0, r1, r2, r3);
; }

; void MultiInstOpt(int a, int b) {
;   int r0 = a + 3;
;   int r1 = r0 - 3;
;   int r2 = a + b;
;   int r3 = r2 - b;
;   printf("%d,%d,%d,%d\n", r0, r1, r2, r3);
; }

@.str = private unnamed_addr constant [19 x i8] c"%d,%d,%d,%d,%d,%d\0A\00", align 1
@.str.1 = private unnamed_addr constant [13 x i8] c"%d,%d,%d,%d\0A\00", align 1

define dso_local void @AlgebraicIdentity(i32 noundef %0) {
; CHECK-LABEL: define dso_local void @AlgebraicIdentity(i32 noundef %0) {
; @todo(CSCD70) Please complete the CHECK directives.
  %2 = add nsw i32 %0, 0
  %3 = add nsw i32 0, %0
  %4 = mul nsw i32 %0, 1
  %5 = mul nsw i32 1, %0
  %6 = sub nsw i32 %0, 0
  %7 = sdiv i32 %0, 1
  %8 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %2, i32 noundef %3, i32 noundef %4, i32 noundef %5, i32 noundef %6, i32 noundef %7)
  ret void
}

define dso_local void @StrengthReduction(i32 noundef %0) {
; CHECK-LABEL: define dso_local void @StrengthReduction(i32 noundef %0) {
; @todo(CSCD70) Please complete the CHECK directives.
  %2 = mul nsw i32 %0, 2
  %3 = mul nsw i32 64, %0
  %4 = sdiv i32 %0, 4
  %5 = sdiv i32 %0, 128
  %6 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %2, i32 noundef %3, i32 noundef %4, i32 noundef %5)
  ret void
}

define dso_local void @MultiInstOpt(i32 noundef %0, i32 noundef %1) {
; CHECK-LABEL: define dso_local void @MultiInstOpt(i32 noundef %0, i32 noundef %1) {
; @todo(CSCD70) Please complete the CHECK directives.
  %3 = add nsw i32 %0, 3
  %4 = sub nsw i32 %3, 3
  %5 = add nsw i32 %0, %1
  %6 = sub nsw i32 %5, %1
  %7 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %3, i32 noundef %4, i32 noundef %5, i32 noundef %6)
  ret void
}

declare i32 @printf(ptr noundef, ...)
