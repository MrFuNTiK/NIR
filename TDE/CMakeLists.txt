project(TDE_calculator)

cmake_minimum_required(VERSION 3.10)

add_library(${PROJECT_NAME} STATIC
            ${CMAKE_CURRENT_SOURCE_DIR}/TDE_class.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/GPS_class.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/GPS_GRZ_class.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/GCC_class.cpp)
target_link_libraries(${PROJECT_NAME} fft)

if(ENABLE_LOGGER)
    target_link_libraries(${PROJECT_NAME} logger)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/core)
