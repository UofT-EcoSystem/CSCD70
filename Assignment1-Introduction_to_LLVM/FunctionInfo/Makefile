OPTIMIZER := FunctionInfo.so
OBJs := $(subst .cpp,.o,$(wildcard lib/*.cpp))

LLVM_VERSION ?= 12
LLVM_VERSION_SUFFIX := -$(LLVM_VERSION)

CXXFLAGS := $(shell llvm-config$(LLVM_VERSION_SUFFIX) --cxxflags) -fPIC

all: $(OPTIMIZER) 

$(OPTIMIZER): $(OBJs)
	$(CXX) -dylib -shared $^ -o $@

.PHONY: clean
clean:
	$(RM) $(OPTIMIZER) $(OBJs)
