; RUN: llc -load %dylibdir/libRegAlloc%dylibext \
; RUN:     -regalloc=cscd70 -march=mips %s -o %basename_t.s
; RUN: FileCheck --match-full-lines %s --input-file=%basename_t.s

; int g;

; int def() {
;   return g++;
; }

; void use(int i) {
;   g = i;
; }

; int foo() {
;   int A = def(), B, C, D;

;   if (A) {
;     B = def();
;     use(A);
;     D = B + 1;
;   } else {
;     C = def();
;     use(A);
;     D = C + 1;
;   }
;   A = def();
;   use(A);
;   return D;
; }

@g = global i32 0, align 4

define i32 @foo() {
  %1 = call i32 @def()
  %2 = icmp ne i32 %1, 0
  br i1 %2, label %3, label %6

3:                                                ; preds = %0
  %4 = call i32 @def()
  call void @use(i32 %1)
  %5 = add nsw i32 %4, 1
  br label %9

6:                                                ; preds = %0
  %7 = call i32 @def()
  call void @use(i32 %1)
  %8 = add nsw i32 %7, 1
  br label %9

9:                                                ; preds = %6, %3
  %.0 = phi i32 [ %5, %3 ], [ %8, %6 ]
  %10 = call i32 @def()
  call void @use(i32 %10)
  ret i32 %.0
}

define i32 @def() {
  %1 = load i32, i32* @g, align 4
  %2 = add nsw i32 %1, 1
  store i32 %2, i32* @g, align 4
  ret i32 %1
}

define void @use(i32 %0) {
  store i32 %0, i32* @g, align 4
  ret void
}
