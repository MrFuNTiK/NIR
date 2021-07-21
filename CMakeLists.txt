project(TDE)

cmake_minimum_required(VERSION 3.16)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)


add_subdirectory(core)
add_subdirectory(fft)
add_subdirectory(TDE)
add_subdirectory(GCC)
add_subdirectory(GPS)

add_executable(tde ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_include_directories(tde PRIVATE  ${CMAKE_SOURCE_DIR}/GPS
                                        ${CMAKE_SOURCE_DIR}/GCC
                                        ${CMAKE_SOURCE_DIR}/TDE)
target_link_libraries(tde GPS_lib GCC_lib TDE_lib core fft fftw3)
