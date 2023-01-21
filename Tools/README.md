# Tools

This folder contains tools that you might find helpful for your development.

- `C_to_LLVM_IR` helps you convert a C source file to its equivalent LLVM IR format.
  ```Bash
  ./C_to_LLVM_IR/C_to_LLVM_IR.sh [-Ox]  # Optimization level
                                 [-p/--passes=xxx (comma separated)]  # Passes to execute
                                 -c file  # C source code to compile
  ```
  E.g.,

  ```Bash
  /Tools$ ./C_to_LLVM_IR/C_to_LLVM_IR.sh -O0 -p=mem2reg -c C_to_LLVM_IR/main.c
  ```
