execute_process(COMMAND llvm-config${LLVM_VERSION_SUFFIX} --bindir
                OUTPUT_VARIABLE LLVM_BINDIR
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_LIBRARY_OUTPUT_DIR "${CMAKE_BINARY_DIR}/lib")

# Configure file lit.cfg.in.py and output to lit.cfg.py. When configuring, all
# variables in the form of @var@ will be replaced by their respective value.
# E.g., @LLVM_BINDIR@ in lit.cfg.in.py will be replaced by `llvm-config --bindir`.
configure_file(lit.cfg.in.py lit.cfg.py @ONLY)

add_test(NAME FunctionInfoTest COMMAND lit -v ${CMAKE_CURRENT_BINARY_DIR})
