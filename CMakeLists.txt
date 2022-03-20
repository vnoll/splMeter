cmake_minimum_required(VERSION 3.10)

# set the project name
project(SoundPressureLevel VERSION 0.0.1)
configure_file(SPLConfig.h.in SPLConfig.h)
# add the executable
add_executable(SoundPressureLevel spl.cpp)
target_include_directories(SoundPressureLevel PUBLIC "${PROJECT_BINARY_DIR}")


