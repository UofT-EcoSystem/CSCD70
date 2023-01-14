cmake_minimum_required(VERSION 3.18.4)
project(TransformPassSample)

set(LLVM_VERSION "16" CACHE STRING "LLVM Version")

execute_process(COMMAND llvm-config-${LLVM_VERSION} --cxxflags
                OUTPUT_VARIABLE LLVM_CXXFLAGS
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${LLVM_CXXFLAGS} -Wall")
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_CLANG_TIDY "clang-tidy-${LLVM_VERSION};--extra-arg-before=-stdlib=libc++")
        message(STATUS "Enabling clang-tidy")
endif()

add_subdirectory(lib)

include(CTest)
enable_testing()
add_subdirectory(test)

file(GLOB_RECURSE SRC_FILEs "*.h" "*.cpp")
add_custom_target(format WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                         COMMAND clang-format${LLVM_VERSION_SUFFIX} -i ${SRC_FILEs})
