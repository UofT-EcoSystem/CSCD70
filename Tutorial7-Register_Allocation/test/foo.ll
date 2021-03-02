; RUN: llc -load %dylibdir/libRegAlloc%dylibext \
; RUN:     -regalloc=cscd70 -march=mips %s -o %basename_t.s
; RUN: FileCheck --match-full-lines %s --input-file=%basename_t.s

; #include <stdio.h>
; #include <stdlib.h>

; int g;

; int def() {
;   return rand();
; }
; void use(int i) {
;   printf("%d", i);
; }
; int copy(int i) {
;   return i * i;
; }

; int foo() {
;   int A = def(), B, C, D;

;   if (A) {
;     B = def();
;     use(A);
;     D = copy(B);
;   } else {
;     C = def();
;     use(A);
;     D = def();
;   }
;   A = def();
;   use(A);
;   return D;
; }

@.str = private constant [3 x i8] c"%d\00", align 1

declare i32 @printf(i8*, ...)
declare i32 @rand()

define i32 @foo() {
  %1 = call i32 @def()
  %2 = icmp ne i32 %1, 0
  br i1 %2, label %3, label %6

3:                                                ; preds = %0
  %4 = call i32 @def()
  call void @use(i32 %1)
  %5 = call i32 @copy(i32 %4)
  br label %9

6:                                                ; preds = %0
  %7 = call i32 @def()
  call void @use(i32 %1)
  %8 = call i32 @def()
  br label %9

9:                                                ; preds = %6, %3
  %.0 = phi i32 [ %5, %3 ], [ %8, %6 ]
  %10 = call i32 @def()
  call void @use(i32 %10)
  ret i32 %.0
}

define i32 @def() {
  %1 = call i32 @rand() #3
  ret i32 %1
}

define void @use(i32 %0) {
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32 %0)
  ret void
}

define i32 @copy(i32 %0) {
  %2 = mul nsw i32 %0, %0
  ret i32 %2
}
