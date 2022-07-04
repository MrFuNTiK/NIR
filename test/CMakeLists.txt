project(tests)

if(ENABLE_TEST)
    cmake_minimum_required(VERSION 3.16)
    find_package(GTest REQUIRED)
    include(GoogleTest)

    add_executable(${PROJECT_NAME} main.cpp calc_test.cpp)
    target_link_libraries(${PROJECT_NAME} util_helper TDE_calculator ${GTEST_LIBRARIES} pthread)

    add_custom_target(  leak-check
                        COMMAND valgrind ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}
                        VERBATIM )

    add_custom_target(  check
                        COMMAND ${PROJECT_NAME}
                        VERBATIM )
endif()
