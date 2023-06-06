project(TDE_TOOLS LANGUAGES C CXX)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(RESULT_DIRECTORY ${CMAKE_SOURCE_DIR}/_result)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RESULT_DIRECTORY}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${RESULT_DIRECTORY}/lib)

option(ENABLE_WAV_FILE_READING CACHE OFF)
option(ENABLE_LOGGER CACHE OFF)
option(ENABLE_TEST CACHE OFF)

message(STATUS "Build profile: ${CMAKE_BUILD_TYPE}")
message(STATUS "Result directory: ${RESULT_DIRECTORY}")
message(STATUS "Reading wav-file: ${ENABLE_WAV_FILE_READING}")
message(STATUS "Enable logger: ${ENABLE_LOGGER}")
message(STATUS "Enable tests: ${ENABLE_TEST}")
message(STATUS "================================================")

include_directories(include internal_include)
add_compile_options(-Wall -Wextra -Werror)

enable_testing()

add_subdirectory(fft)
add_subdirectory(TDE)
add_subdirectory(rt_audio)
add_subdirectory(cmd_util)
add_subdirectory(test)
add_subdirectory(util_helper)
add_subdirectory(logger)
