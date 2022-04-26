project(PROGRAM_ENVIRONMENT)

cmake_minimum_required(VERSION 3.16)

add_library(program_environment STATIC
            ${CMAKE_CURRENT_SOURCE_DIR}/program_environment.cpp)
target_include_directories(TDE_lib PRIVATE ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/TDE
                                           ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/GCC
                                           ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/GPS)
