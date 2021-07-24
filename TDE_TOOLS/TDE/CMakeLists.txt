project(TDE_BASIC)

cmake_minimum_required(VERSION 3.16)

add_library(TDE_lib STATIC
            ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/TDE/TDE_class.cpp)
target_include_directories(TDE_lib PRIVATE ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/core)