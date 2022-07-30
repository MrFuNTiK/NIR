project(cmd_util)

cmake_minimum_required(VERSION 3.10)

add_executable(${PROJECT_NAME}
            ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(${PROJECT_NAME} util_helper TDE_calculator)

if(${ENABLE_WAV_FILE_READING})
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_WAV_FILE_READING)
endif()
