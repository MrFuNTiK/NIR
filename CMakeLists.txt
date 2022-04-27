project(TDE_TOOLS)

cmake_minimum_required(VERSION 3.16)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
include_directories(include internal_include)

add_subdirectory(fft)
add_subdirectory(TDE)
add_subdirectory(cmd_util)
