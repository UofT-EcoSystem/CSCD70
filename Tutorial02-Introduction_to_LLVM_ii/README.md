# Tutorial 2 Introduction to LLVM ii

**Handout**: [\[Overleaf\]](https://www.overleaf.com/read/vdwnnwdcshyx)

## TL'DR

- User-Use-Value Relationship
- LLVM Pass Manager: Require & Preserve

## Examples

1. `Transform_Pass_Example` shows you how to traverse through the users and
   operands of an instruction, and also how to insert new instructions into the
   module.
   ```Bash
   /Example1-Transform_Pass_Example$ mkdir build && cd build
   /Example1-Transform_Pass_Example/build$ cmake -DCMAKE_BUILD_TYPE=Release ..
   /Example1-Transform_Pass_Example/build$ make
   /Example1-Transform_Pass_Example/build$ ctest -V
   ```
1. `Pass_Manager` shows you how to require and preserve/abandon other analysis
   passes.
   ```Bash
   /Example1-Pass_Manager$ mkdir build && cd build
   /Example1-Pass_Manager/build$ cmake -DCMAKE_BUILD_TYPE=Release ..
   /Example1-Pass_Manager/build$ make
   /Example1-Pass_Manager/build$ ctest -V
   ```
