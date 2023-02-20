cmake_minimum_required(VERSION 3.18.4)
project(LocalOpts)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LLVM_VERSION
    "16"
    CACHE STRING "LLVM Version")

execute_process(
  COMMAND llvm-config-${LLVM_VERSION} --cxxflags
  OUTPUT_VARIABLE LLVM_CXXFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE)
add_compile_options("${LLVM_CXXFLAGS}")
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_CLANG_TIDY
      "clang-tidy-${LLVM_VERSION};--extra-arg-before=-stdlib=libc++")
  message(STATUS "Enabling clang-tidy")

  execute_process(
    COMMAND llvm-config-${LLVM_VERSION} --includedir
    OUTPUT_VARIABLE LLVM_INCLUDEDIR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  add_compile_options("-isystem ${LLVM_INCLUDEDIR} -Wall -Wextra -Werror")
endif()

add_subdirectory(lib)

include(CTest)
enable_testing()
add_subdirectory(test)
