# Tutorial 6 Scalar Evolution

**Handout**: [\[Overleaf\]](https://www.overleaf.com/read/qyzvpfzxkrns)

## TL'DR

- Scalar Evolution
  - Chain of Recurrences
  - Strength Reduction
  - Folding Chains of Recurrences
  - SCEV in LLVM

## Examples

The example shows you how to use the `print<scalar-evolution>` pass of LLVM to
print interesting information of loop variables in a program, and furthermore,
how to build on top of the `ScalarEvolution` pass by obtaining its result within
a function pass.
```
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
$ ctest -V
```
