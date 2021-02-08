CC := g++

CXXFLAGS := -std=c++11

EXEs := $(basename $(wildcard *.cpp))
all: $(EXEs)

%: %.cpp

.PHONY: format
format: *.cpp
	clang-format -i $^

.PHONY: clean
clean:
	$(RM) main
