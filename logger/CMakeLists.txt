project(logger)

cmake_minimum_required(VERSION 3.10)

if(ENABLE_LOGGER)
    if( ${CMAKE_BUILD_TYPE} STREQUAL "Release" )
        message(NOTICE "!!!ATTENTION!!!")
        message(NOTICE "Logger is enabled in release build type")
    endif()
    add_library(${PROJECT_NAME} STATIC
                ${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp)

    target_compile_definitions(${PROJECT_NAME} INTERFACE "ENABLE_LOGGER")

endif()
