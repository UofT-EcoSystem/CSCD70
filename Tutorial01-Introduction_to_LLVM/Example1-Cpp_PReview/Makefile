CC := g++

CXXFLAGS := -std=c++11 -Wall -Werror

EXEs := $(basename $(wildcard *.cpp))
all: $(EXEs)

%: %.cpp

.PHONY: format
format: *.cpp
	clang-format -i $^

.PHONY: tidy
tidy: *.cpp
	clang-tidy --header-filter=.* $^ --

.PHONY: clean
clean:
	$(RM) main
