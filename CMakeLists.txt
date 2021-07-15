project(CORRELATION)

cmake_minimum_required(VERSION 3.16)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
#add_compile_definitions(WAV_FILE_PATH="/home/kirill/Study/NIR/4.wav")


add_library(core OBJECT ${CMAKE_SOURCE_DIR}/core/core.cpp)

add_library(fft OBJECT
            ${CMAKE_SOURCE_DIR}/fft/fft_forward_class.cpp
            ${CMAKE_SOURCE_DIR}/fft/fft_reverse_class.cpp)
target_include_directories(fft PRIVATE ${CMAKE_SOURCE_DIR}/core)
#target_link_libraries(fft fftw3)

add_library(GCC_lib OBJECT
            ${CMAKE_SOURCE_DIR}/GCC/GCC_class.cpp)
target_include_directories(GCC_lib PRIVATE  ${CMAKE_SOURCE_DIR}/fft
                                            ${CMAKE_SOURCE_DIR}/core)

add_library(GPS_lib OBJECT
            ${CMAKE_SOURCE_DIR}/GPS/GPS_class.cpp)
target_include_directories(GPS_lib PRIVATE  ${CMAKE_SOURCE_DIR}/fft
                                                ${CMAKE_SOURCE_DIR}/core)

add_executable(correlation ${CMAKE_SOURCE_DIR}/src/correlation.cpp)
target_include_directories(correlation PRIVATE ${CMAKE_SOURCE_DIR}/GCC)
target_link_libraries(correlation GCC_lib core fft fftw3)

add_executable(phase ${CMAKE_SOURCE_DIR}/src/phase.cpp)
target_include_directories(phase PRIVATE ${CMAKE_SOURCE_DIR}/GPS)
target_link_libraries(phase GPS_lib core fft fftw3)
