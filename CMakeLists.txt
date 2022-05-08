project(TDE_TOOLS)

cmake_minimum_required(VERSION 3.16)

set(RESULT_DIRECTORY ${CMAKE_SOURCE_DIR}/result)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RESULT_DIRECTORY}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${RESULT_DIRECTORY}/lib)
set(ENABLE_WAV_FILE_READING false)

message(STATUS "Build profile: ${CMAKE_BUILD_TYPE}")
message(STATUS "Result directory: ${RESULT_DIRECTORY}")
message(STATUS "Reading wav-file: ${ENABLE_WAV_FILE_READING}")

include_directories(include internal_include)

add_subdirectory(fft)
add_subdirectory(TDE)
add_subdirectory(cmd_util)
add_subdirectory(rt_audio)
add_subdirectory(util_helper)
