project(FFT)

cmake_minimum_required(VERSION 3.16)


add_library(fft STATIC
            ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/fft/fft_forward_class.cpp
            ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/fft/fft_reverse_class.cpp)
target_include_directories(fft PRIVATE ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/core)