LLs := $(subst .c,.ll,$(wildcard *.c))

LLVM_VERSION ?= 12
LLVM_VERSION_SUFFIX := -$(LLVM_VERSION)

.PHONY: all
all: $(LLs)

%.ll: %.bc
	opt$(LLVM_VERSION_SUFFIX) -S -mem2reg $< -o $@

%.bc: %.c
	clang$(LLVM_VERSION_SUFFIX) -O0 -Xclang -disable-O0-optnone -emit-llvm -c $< -o $*.bc

.PHONY: clean
clean:
	$(RM) $(LLs)
