cmake_minimum_required(VERSION 3.12.0)
project(experimental-llvm_9)

if (NOT "$ENV{LLVM_VERSION}" STREQUAL "")
    set(LLVM_VERSION $ENV{LLVM_VERSION})
else()
    set(LLVM_VERSION 9.0)
endif()

execute_process (
    COMMAND llvm-config-${LLVM_VERSION} --cxxflags
    OUTPUT_VARIABLE LLVM_CONFIG_CXXFLAGS
)

message("${LLVM_CONFIG_CXXFLAGS}")
