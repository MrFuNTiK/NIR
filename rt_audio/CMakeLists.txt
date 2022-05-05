project(rt_audio)

cmake_minimum_required(VERSION 3.16)

add_library(${PROJECT_NAME} STATIC RtAudio.cpp rtaudio_c.cpp)
target_compile_definitions(rt_audio PRIVATE "__LINUX_ALSA__")
target_link_libraries(rt_audio asound)
