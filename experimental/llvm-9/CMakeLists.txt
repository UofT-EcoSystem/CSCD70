cmake_minimum_required(VERSION 3.12.0)
project(experimental-llvm_9)

if (NOT "$ENV{LLVM_VERSION}" STREQUAL "")
    set(LLVM_VERSION $ENV{LLVM_VERSION})
else()
    set(LLVM_VERSION 9.0)
endif()

execute_process(COMMAND llvm-config-${LLVM_VERSION} --cxxflags
                OUTPUT_VARIABLE LLVM_CONFIG_CXXFLAGS
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS ${LLVM_CONFIG_CXXFLAGS})

add_subdirectory(lib)
include(CTest)
enable_testing()
add_subdirectory(test)
