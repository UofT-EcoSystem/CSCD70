CC := g++

CXXFLAGS := -std=c++11

EXEs := $(basename $(wildcard *.cpp))
all: $(EXEs)

%: %.cpp

.PHONY: clean
clean:
	$(RM) main
