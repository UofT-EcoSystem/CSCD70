CC := g++
CXXFLAGS := -std=c++11 -Wall

EXES := $(basename $(wildcard *.cpp))
all: $(EXES)

%: %.cpp

.PHONY: format tidy clean
format: *.cpp
	clang-format -i $^

tidy: *.cpp
	clang-tidy --header-filter=.* $^ --

clean:
	$(RM) $(EXES)
