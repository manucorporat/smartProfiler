cmake_minimum_required(VERSION 1.0)
project( PROFILER )

set(CMAKE_C_FLAGS "-O2")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Make sure the compiler can find include files from our Hello library. 
include_directories (${PROFILER_SOURCE_DIR}) 

add_executable (test test.c) 