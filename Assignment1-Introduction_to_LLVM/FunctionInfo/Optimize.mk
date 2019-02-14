OPTIMIZER := FunctionInfo.so
OPT_PASSES := -function-info

LLVM_VERSION ?= 6.0

CXXFLAGS := $(shell llvm-config-$(LLVM_VERSION) --cxxflags) -rdynamic -g -std=c++17

OPT_OBJs := $(subst .cpp,.o,$(wildcard *.cpp))

TEST_SRCs := $(basename $(notdir $(wildcard ./tests/*.c)))
TEST_RAW_LLs := $(subst .c,.ll,$(wildcard ./tests/*.c))
TEST_OPT_LLs := $(addprefix ./tests/,$(addsuffix -opt.ll,$(TEST_SRCs)))

all: $(TEST_OPT_LLs) $(TEST_RAW_LLs) 

# Every disassembly file depends on the corresponding bytecode.
./tests/%.ll: ./tests/%.bc
	llvm-dis-$(LLVM_VERSION) $< -o=$@

# Every optimized bytecode depends on the corresponding raw bytecode and the optimizer.
./tests/%-opt.bc: ./tests/%.bc $(OPTIMIZER)
	env LD_LIBRARY_PATH=. opt-$(LLVM_VERSION) -load $(OPTIMIZER) $(OPT_PASSES) $< -o $@

# Every raw bytecode is built from the corresponding C source file.
# You can also use the option `-O0 -Xclang -disable-O0-optnone` 
# (rather than `-O2`) to disable Clang native optimizations.
./tests/%.bc: ./tests/%.c
	clang-$(LLVM_VERSION) -O2 -emit-llvm -c $< -o $@

# Build the optimizer from the source files.
$(OPTIMIZER): $(OPT_OBJs)
	$(CXX) -dylib -shared $^ -o $@

.PHONY: clean
clean:
	$(RM) $(TEST_OPT_LLs) $(TEST_RAW_LLs) $(OPTIMIZER) $(OPT_OBJs)