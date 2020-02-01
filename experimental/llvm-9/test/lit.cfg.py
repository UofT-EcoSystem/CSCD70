import lit
from lit.llvm import llvm_config

import os


config.name = 'LLVM-9'
config.test_format = lit.formats.ShTest(not llvm_config.use_lit_shell)
config.suffixes = ['.c', '.ll']

llvm_config.add_tool_substitutions(
        ["opt", "lli", "not", "FileCheck", "clang"],
        config.llvm_config_bindir)
