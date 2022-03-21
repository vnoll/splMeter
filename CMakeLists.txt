cmake_minimum_required(VERSION 3.10)

# set the project name
project(SoundPressureLevel VERSION 0.0.2)
configure_file(SPLConfig.h.in SPLConfig.h)
add_compile_options(-pthread)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
file(GLOB SRC_FILES
	"src/*.cpp"
    "src/*.c"
)

file(GLOB LIB_FILES
        "lib/*.a"
        "lib/*.so"
        "lib/*.la")

find_package(Threads REQUIRED)
find_package(ALSA REQUIRED)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${LIB_FILES} Threads::Threads ${ALSA_LIBRARIES})

include_directories(${PROJECT_NAME} PUBLIC 
                                    "${PROJECT_BINARY_DIR}"
                                    "${PROJECT_SOURCE_DIR}/lib/"
                                    "${PROJECT_SOURCE_DIR}/src/"
                                    "${PROJECT_SOURCE_DIR}/inc/")





