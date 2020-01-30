import lit
from lit.llvm import llvm_config
import os


config.name = 'LLVM-9'
# the Test Format used to Interpret Tests
config.test_format = lit.formats.ShTest(not llvm_config.use_lit_shell)
config.suffixes = ['.ll']
config.test_source_root = os.path.dirname(__file__)
config.excludes = ['Inputs']

llvm_config.add_tool_substitutions(
        ["opt", "lli", "not", "FileCheck", "clang"],
        config.llvm_tools_dir)

config.substitutions.append(('%dylibext', config.llvm_dylib_ext))
config.substitutions.append(('%dylibdir', config.llvm_dylib_dir))
