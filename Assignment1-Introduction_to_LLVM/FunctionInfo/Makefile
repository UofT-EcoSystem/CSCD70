OPTIMIZER := FunctionInfo.so
OBJs := $(subst .cpp,.o,$(wildcard lib/*.cpp))

CXXFLAGS := $(shell llvm-config --cxxflags) -fPIC

all: $(OPTIMIZER) 

$(OPTIMIZER): $(OBJs)
	$(CXX) -dylib -shared $^ -o $@

.PHONY: clean
clean:
	$(RM) $(OPTIMIZER) $(OBJs)
