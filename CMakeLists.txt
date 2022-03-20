cmake_minimum_required(VERSION 3.10)

# set the project name
project(SoundPressureLevel VERSION 0.0.1)
configure_file(SPLConfig.h.in SPLConfig.h)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
file(GLOB SRC_FILES
	"src/*.cpp"
)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libportaudio.a)

include_directories(${PROJECT_NAME} PUBLIC 
                                    "${PROJECT_BINARY_DIR}"
                                    "${PROJECT_SOURCE_DIR}/lib/"
                                    "${PROJECT_SOURCE_DIR}/src/"
                                    "${PROJECT_SOURCE_DIR}/inc/")





