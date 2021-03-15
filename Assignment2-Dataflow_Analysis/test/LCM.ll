; RUN: opt -S -load %dylibdir/libDFA.so -anti-expr \
; RUN:     %s -o %basename_t 2>%basename_t.antiexpr.log
; RUN: FileCheck --match-full-lines %s --input-file=%basename_t.antiexpr.log \
; RUN:     --check-prefix=ANTIEXPR

; RUN: opt -S -load %dylibdir/libDFA.so -wb-avail-expr \
; RUN:     %s -o %basename_t 2>%basename_t.wbavailexpr.log
; RUN: FileCheck --match-full-lines %s --input-file=%basename_t.wbavailexpr.log \
; RUN:     --check-prefix=WBAVAILEXPR

; RUN: opt -S -load %dylibdir/libDFA.so -eplace \
; RUN:     %s -o %basename_t 2>%basename_t.eplace.log
; RUN: FileCheck --match-full-lines %s --input-file=%basename_t.eplace.log \
; RUN:     --check-prefix=EPLACE

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
; ANTIEXPR-LABEL: * Anticipated Expression *

; WBAVAILEXPR-LABEL: * Will-Be-Available Expression *

; EPLACE-LABEL: * Earliest Placement *

; @todo(cscd70) Please complete the CHECK directives.
define i32 @foo(i32 %0, i32 %1, i32 %2) {
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
