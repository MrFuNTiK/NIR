project(GCC)

cmake_minimum_required(VERSION 3.16)

add_library(GCC_lib STATIC
            ${CMAKE_SOURCE_DIR}/GCC/GCC_class.cpp)
target_include_directories(GCC_lib PRIVATE  ${CMAKE_SOURCE_DIR}/fft
                                            ${CMAKE_SOURCE_DIR}/core
                                            ${CMAKE_SOURCE_DIR}/TDE)