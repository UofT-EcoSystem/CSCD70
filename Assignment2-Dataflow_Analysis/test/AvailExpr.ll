define i32 @AvailExpr(i32 %0, i8** %1) #0 {
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
