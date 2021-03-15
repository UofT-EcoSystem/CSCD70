# *lit* is the LLVM integrated tester. Please visit
# 
#     https://llvm.org/docs/CommandGuide/lit.html
# 
# for its documentation.
import lit
import lit.llvm

lit.llvm.initialize(lit_config, config)

from lit.llvm import llvm_config


config.name = 'FunctionInfo'
config.test_format = lit.formats.ShTest()
                     # The ShTest files contain some number of shell-like
                     # command pipelines, along with assertions about what
                     # should be in the output.
config.test_source_root = "@CMAKE_CURRENT_SOURCE_DIR@"
config.test_exec_root   = "@CMAKE_CURRENT_BINARY_DIR@"
config.suffixes = ['.c', '.ll']

config.substitutions.append((r'%dylibdir', "@CMAKE_LIBRARY_OUTPUT_DIR@"))

# Extra config attribute for tool substitutions.
# FileCheck is not in the default executable path, therefore we need to add it manually.
config.llvm_config_bindir = "@LLVM_BINDIR@"
llvm_config.add_tool_substitutions(
        ["clang", "opt", "FileCheck"],
        config.llvm_config_bindir)
