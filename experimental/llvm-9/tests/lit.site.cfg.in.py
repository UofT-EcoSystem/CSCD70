import lit
import lit.llvm
from lit.llvm import llvm_config

import os

config.llvm_tools_dir = "@LLVM_CONFIG_BINDIR@"
config.llvm_dylib_ext = "@CMAKE_SHARED_LIBRARY_SUFFIX@"
config.llvm_dylib_dir = "@CMAKE_LIBRARY_OUTPUT_DIR@"
config.test_exec_root = os.path.join("@CMAKE_CURRENT_BINARY_DIR@")

lit.llvm.initialize(lit_config, config)
lit_config.load_config(config, "@CMAKE_CURRENT_SOURCE_DIR@/lit.cfg.py")
