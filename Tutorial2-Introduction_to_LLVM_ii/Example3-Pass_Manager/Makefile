OPTIMIZER := MyPass.so
OPT_PASSES := -transform -another-transform

LLVM_VERSION := 6.0

CXXFLAGS := $(shell llvm-config-$(LLVM_VERSION) --cxxflags) -rdynamic -g -std=c++17

OPT_OBJs := $(subst .cpp,.o,$(wildcard *.cpp))

TEST_SRCs := $(basename $(notdir $(wildcard ./tests/*.c)))
TEST_RAW_LLs := $(subst .c,.ll,$(wildcard ./tests/*.c))
TEST_OPT_LLs := $(addprefix ./tests/,$(addsuffix -opt.ll,$(TEST_SRCs)))

all: $(TEST_OPT_LLs) $(TEST_RAW_LLs) 

./tests/%.ll: ./tests/%.bc
	llvm-dis-$(LLVM_VERSION) $< -o=$@

./tests/%-opt.bc: ./tests/%.bc $(OPTIMIZER)
	env LD_LIBRARY_PATH=. opt-$(LLVM_VERSION) -load $(OPTIMIZER) $(OPT_PASSES) $< -o $@

./tests/%.bc: ./tests/%.c
	clang-$(LLVM_VERSION) -O2 -emit-llvm -c $< -o $@

$(OPTIMIZER): $(OPT_OBJs)
	$(CXX) -dylib -shared $^ -o $@
.INTERMEDIATE: $(OPTIMIZER) $(OPT_OBJs)

.PHONY: clean
clean:
	$(RM) $(TEST_OPT_LLs) $(TEST_RAW_LLs) 