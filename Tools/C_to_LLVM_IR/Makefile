LLs := $(subst .c,.ll,$(wildcard *.c))

.PHONY: all
all: $(LLs)

%.ll: %.bc
	llvm-dis $< -o=$@

%.bc: %.c
	clang -O0 -Xclang -disable-O0-optnone -emit-llvm -c $< -o $@
	opt -mem2reg $@
	

.PHONY: clean
clean:
	$(RM) $(LLs)
