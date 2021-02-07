LLs := $(subst .c,.ll,$(wildcard *.c))

.PHONY: all
all: $(LLs)

%.ll: %.bc
	opt -S -mem2reg $< -o $@

%.bc: %.c
	clang -O0 -Xclang -disable-O0-optnone -emit-llvm -c $< -o $*.bc

.PHONY: clean
clean:
	$(RM) $(LLs)
