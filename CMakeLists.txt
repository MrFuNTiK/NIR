project(TDE_TOOLS)

cmake_minimum_required(VERSION 3.16)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)


add_subdirectory(${CMAKE_PROJECT_NAME}/core)
add_subdirectory(${CMAKE_PROJECT_NAME}/fft)
add_subdirectory(${CMAKE_PROJECT_NAME}/TDE)
add_subdirectory(${CMAKE_PROJECT_NAME}/GCC)
add_subdirectory(${CMAKE_PROJECT_NAME}/GPS)
add_subdirectory(${CMAKE_PROJECT_NAME}/program_environment)

add_executable(tde ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/main.cpp)
target_include_directories(tde PRIVATE  ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/GPS
                                        ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/GCC
                                        ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/TDE)
target_link_libraries(tde program_environment GPS_lib GCC_lib TDE_lib core fft fftw3)
