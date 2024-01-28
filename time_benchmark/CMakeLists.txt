project(benchmark)

cmake_minimum_required(VERSION 3.10)

add_executable(time_bench
            ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_executable(mem_bench
            ${CMAKE_CURRENT_SOURCE_DIR}/mem_bench.cpp)

target_link_libraries(time_bench TDE_calculator)
target_link_libraries(mem_bench TDE_calculator)

if(ENABLE_LOGGER)
    target_link_libraries(time_bench logger)
    target_link_libraries(mem_bench logger)
endif()
