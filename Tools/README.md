# Tools

This folder contains tools that you might find helpful for your development.

###### `C_to_LLVM_IR`

```Bash
./C_to_LLVM_IR/C_to_LLVM_IR.sh [-Ox] [--passes=xxx (semi-colon separated)] -c file
```

E.g.,

```Bash
./C_to_LLVM_IR/C_to_LLVM_IR.sh -O0 --passes=mem2reg -c C_to_LLVM_IR/main.c
```
