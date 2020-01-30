execute_process (
    COMMAND llvm-config-${LLVM_VERSION} --bindir
    OUTPUT_VARIABLE LLVM_CONFIG_BINDIR
    OUTPUT_STRIP_TRAILING_WHITESPACE  # remove the trailing newline character
)

configure_file(lit.site.cfg.in.py
               lit.site.cfg.py @ONLY)
