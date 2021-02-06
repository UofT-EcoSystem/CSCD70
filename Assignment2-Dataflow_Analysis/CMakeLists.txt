cmake_minimum_required(VERSION 3.12.0)
project(DFA)

execute_process(COMMAND llvm-config --cxxflags
                OUTPUT_VARIABLE LLVM_CXXFLAGS
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS ${LLVM_CXXFLAGS})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_CLANG_TIDY clang-tidy)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(lib)
