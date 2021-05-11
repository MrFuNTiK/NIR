cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE OFF)
project(FFTW)

set(CMAKE_CXX_STANDARD 11)

add_compile_definitions(SIGNAL=1) #1 - read from file, 2 - sin signal, 3 imp
add_compile_definitions(SAMPLE_DELAY=-1)

add_executable(phase ${CMAKE_SOURCE_DIR}/source/phase.cpp)
target_compile_options(phase PRIVATE -I ${CMAKE_SOURCE_DIR}/include/inc)

add_executable(correlation ${CMAKE_SOURCE_DIR}/source/correlation.cpp)
target_compile_options(correlation PRIVATE -I ${CMAKE_SOURCE_DIR}/include/inc)

add_library(lib
        OBJECT
        ${CMAKE_SOURCE_DIR}/include/inc/lib.hpp
        ${CMAKE_SOURCE_DIR}/include/src/lib.cpp)
target_compile_options(lib PRIVATE -I ${CMAKE_SOURCE_DIR}/include/inc)

target_link_libraries(phase lib fftw3)
target_link_libraries(correlation lib fftw3)