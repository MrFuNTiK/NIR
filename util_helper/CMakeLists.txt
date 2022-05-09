project(util_helper)

cmake_minimum_required(VERSION 3.16)

add_library(${PROJECT_NAME} STATIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/program_environment.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/sound_provider.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/sigint.cpp)
target_link_libraries(${PROJECT_NAME} rt_audio pthread)
