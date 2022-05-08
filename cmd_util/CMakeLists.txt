project(cmd_util)

cmake_minimum_required(VERSION 3.16)

add_executable(${PROJECT_NAME}
            ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/program_environment.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sound_provider.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sigint.cpp)
target_link_libraries(${PROJECT_NAME} TDE_calculator rt_audio pthread)

if(${ENABLE_WAV_FILE_READING})
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_WAV_FILE_READING)
endif()
