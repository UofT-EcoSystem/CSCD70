cmake_minimum_required(VERSION 3.18.4)
project(DFA)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LLVM_VERSION "16" CACHE STRING "LLVM Version")

execute_process(COMMAND llvm-config-${LLVM_VERSION} --cxxflags
                OUTPUT_VARIABLE LLVM_CXXFLAGS
                OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND llvm-config-${LLVM_VERSION} --includedir
                OUTPUT_VARIABLE LLVM_INCLUDEDIR
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${LLVM_CXXFLAGS} -isystem ${LLVM_INCLUDEDIR} -Wall")
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_CLANG_TIDY "clang-tidy-${LLVM_VERSION};-header-filter=.*;--extra-arg-before=-stdlib=libc++")
        message(STATUS "Enabling clang-tidy")
endif()
include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(lib)

include(CTest)
enable_testing()
add_subdirectory(test)

file(GLOB_RECURSE SRC_FILEs "*.h" "*.cpp")
add_custom_target(format WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                         COMMAND clang-format${LLVM_VERSION_SUFFIX} -i ${SRC_FILEs})
