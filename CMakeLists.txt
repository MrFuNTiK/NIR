project(TDE_TOOLS)

cmake_minimum_required(VERSION 3.16)

set(RESULT_DIRECTORY ${CMAKE_SOURCE_DIR}/result)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RESULT_DIRECTORY}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${RESULT_DIRECTORY}/lib)

message(STATUS "Build profile: ${CMAKE_BUILD_TYPE}")
message(STATUS "Result directory: ${RESULT_DIRECTORY}")

include_directories(include internal_include)
add_compile_options(-g -Wall -Wextra)

add_subdirectory(fft)
add_subdirectory(TDE)
add_subdirectory(cmd_util)
add_subdirectory(rt_audio)
