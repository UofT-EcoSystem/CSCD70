# *lit* is the LLVM integrated tester. Please visit
#     https://llvm.org/docs/CommandGuide/lit.html
# for its documentation.
execute_process(COMMAND llvm-config --bindir
                OUTPUT_VARIABLE LLVM_CONFIG_BINDIR
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_LIBRARY_OUTPUT_DIR "${CMAKE_BINARY_DIR}/lib")
configure_file(lit.cfg.in.py lit.cfg.py @ONLY)
# configure_file(lit.site.cfg.in.py lit.site.cfg.py @ONLY)

add_test(NAME FunctionInfo COMMAND lit -v ${CMAKE_CURRENT_BINARY_DIR})
