execute_process(COMMAND llvm-config-${LLVM_VERSION} --bindir
                OUTPUT_VARIABLE LLVM_CONFIG_BINDIR
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_LIBRARY_OUTPUT_DIR "${CMAKE_BINARY_DIR}/lib")
# write to the configuration file
configure_file(lit.site.cfg.in.py lit.site.cfg.py @ONLY)

add_test(NAME lit COMMAND lit -v ${CMAKE_CURRENT_BINARY_DIR})
