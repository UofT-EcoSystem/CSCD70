// RUN: clang -O2 -S -emit-llvm -c %s -o %basename_t.ll
// RUN: opt -load %dylibdir/libFunctionInfo.so -function-info -disable-output 2>&1 %basename_t.ll | \
// RUN: FileCheck --match-full-lines --check-prefix=SAMPLE %s
/**
 * @todo(cscd70) Please Remove the `--check-prefix=SAMPLE` option and add the
 *               CHECK directives similar to those in Loop.c.
 */
// SAMPLE: CSCD70 Function Information Pass
#include <stdio.h>

int printf(const char *format, ...) {
  int ret;
  va_list args;
  va_start(args, format);
  ret = vfprintf(stdout, format, args);
  va_end(args);

  return ret;
}

int Fibonacci(const int n) {
  if (n == 0) {
    printf("f(0) = 0");
    return 0;
  }
  if (n == 1) {
    printf("f(1) = 1");
    return 1;
  }
  printf("f(%d) = f(%d) + f(%d)", n, n - 1, n - 2);
  return Fibonacci(n - 1) + Fibonacci(n - 2);
}
