LLs := $(subst .c,.ll,$(wildcard *.c))

EXTRA_PASSES ?= mem2reg
EXTRA_PASSES_OPT_ARG := $(foreach pass,$(EXTRA_PASSES),-$(pass))

$(info $$EXTRA_PASSES_OPT_ARG=[${EXTRA_PASSES_OPT_ARG}])

LLVM_VERSION ?= 12
LLVM_VERSION_SUFFIX := -$(LLVM_VERSION)

.PHONY: all
all: $(LLs) Makefile

%.ll: %.bc
	opt$(LLVM_VERSION_SUFFIX) -S $(EXTRA_PASSES_OPT_ARG) $< -o $@

%.bc: %.c
	clang$(LLVM_VERSION_SUFFIX) -O0 -Xclang -disable-O0-optnone -emit-llvm -c $< -o $*.bc

.PHONY: clean
clean:
	$(RM) $(LLs)
