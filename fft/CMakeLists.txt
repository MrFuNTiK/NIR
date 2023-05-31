project(fft)

cmake_minimum_required(VERSION 3.10)


add_library(${PROJECT_NAME} STATIC
            ${CMAKE_CURRENT_SOURCE_DIR}/fft_forward_class.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/fft_reverse_class.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/core.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/goerzel.c
            ${CMAKE_CURRENT_SOURCE_DIR}/grz_forward_class.cpp)
target_link_libraries(${PROJECT_NAME} fftw3)
if(ENABLE_LOGGER)
    target_link_libraries(${PROJECT_NAME} logger)
endif()
