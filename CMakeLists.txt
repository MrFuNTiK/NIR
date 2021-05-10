cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE OFF)
project(FFTW)

set(CMAKE_CXX_STANDARD 11)

add_executable(main main.cpp)
target_compile_options(main PRIVATE -I ${CMAKE_SOURCE_DIR}/include/inc)

add_library(lib
        OBJECT
        ${CMAKE_SOURCE_DIR}/include/inc/lib.hpp
        ${CMAKE_SOURCE_DIR}/include/src/lib.cpp)
target_compile_options(lib PRIVATE -I ${CMAKE_SOURCE_DIR}/include/inc)

target_link_libraries(main lib fftw3)