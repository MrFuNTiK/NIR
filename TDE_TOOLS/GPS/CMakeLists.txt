project(GPS)

cmake_minimum_required(VERSION 3.16)

add_library(GPS_lib STATIC
            ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/GPS/GPS_class.cpp)
target_include_directories(GPS_lib PRIVATE  ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/fft
                                            ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/core
                                            ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/TDE)