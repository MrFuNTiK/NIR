project(CORRELATION)

cmake_minimum_required(VERSION 3.16)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_library(core OBJECT ${CMAKE_SOURCE_DIR}/core/core.cpp)

add_library(fft OBJECT
            ${CMAKE_SOURCE_DIR}/fft/fft_forward_class.cpp
            ${CMAKE_SOURCE_DIR}/fft/fft_reverse_class.cpp)
target_include_directories(fft PRIVATE ${CMAKE_SOURCE_DIR}/core)

add_library(TDE_lib OBJECT
            ${CMAKE_SOURCE_DIR}/TDE/TDE_class.cpp)
target_include_directories(TDE_lib PRIVATE ${CMAKE_SOURCE_DIR}/core)

add_library(GCC_lib OBJECT
            ${CMAKE_SOURCE_DIR}/GCC/GCC_class.cpp)
target_include_directories(GCC_lib PRIVATE  ${CMAKE_SOURCE_DIR}/fft
                                            ${CMAKE_SOURCE_DIR}/core
                                            ${CMAKE_SOURCE_DIR}/TDE)

add_library(GPS_lib OBJECT
            ${CMAKE_SOURCE_DIR}/GPS/GPS_class.cpp)
target_include_directories(GPS_lib PRIVATE  ${CMAKE_SOURCE_DIR}/fft
                                            ${CMAKE_SOURCE_DIR}/core
                                            ${CMAKE_SOURCE_DIR}/TDE)

add_executable(tde ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_include_directories(tde PRIVATE  ${CMAKE_SOURCE_DIR}/GPS
                                        ${CMAKE_SOURCE_DIR}/GCC
                                        ${CMAKE_SOURCE_DIR}/TDE)
target_link_libraries(tde GPS_lib GCC_lib TDE_lib core fft fftw3)
