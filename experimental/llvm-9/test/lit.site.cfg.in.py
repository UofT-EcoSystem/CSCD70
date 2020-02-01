import lit.llvm


config.test_source_root = "@CMAKE_CURRENT_SOURCE_DIR@"
config.test_exec_root   = "@CMAKE_CURRENT_BINARY_DIR@"
config.llvm_config_bindir = "@LLVM_CONFIG_BINDIR@"
config.substitutions.append(('%dylibext', "@CMAKE_SHARED_LIBRARY_SUFFIX@"))
config.substitutions.append(('%dylibdir', "@CMAKE_LIBRARY_OUTPUT_DIR@"))

lit.llvm.initialize(lit_config, config)
lit_config.load_config(config, "@CMAKE_CURRENT_SOURCE_DIR@/lit.cfg.py")
