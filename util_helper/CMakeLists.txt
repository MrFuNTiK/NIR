project(util_helper)

cmake_minimum_required(VERSION 3.10)

add_library(${PROJECT_NAME} STATIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/program_environment.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/sound_provider.cpp)
target_link_libraries(${PROJECT_NAME} rt_audio pthread)

if(ENABLE_LOGGER)
    target_link_libraries(${PROJECT_NAME} logger)
endif()
