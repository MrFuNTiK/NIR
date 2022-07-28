project(logger)

cmake_minimum_required(VERSION 3.16)

add_library(${PROJECT_NAME} STATIC
            ${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp)
