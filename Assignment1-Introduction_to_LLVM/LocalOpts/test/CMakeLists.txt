execute_process(COMMAND llvm-config${LLVM_VERSION_SUFFIX} --bindir
                OUTPUT_VARIABLE LLVM_BINDIR
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_LIBRARY_OUTPUT_DIR "${CMAKE_BINARY_DIR}/lib")

configure_file(lit.cfg.in.py lit.cfg.py @ONLY)

add_test(NAME LocalOptsTest COMMAND lit -v ${CMAKE_CURRENT_BINARY_DIR})
