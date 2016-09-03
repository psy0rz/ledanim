cmake_minimum_required(VERSION 2.8)

set (CMAKE_CXX_STANDARD 11)

project(ledsim)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} src lib)

set(SOURCE_FILES src/main.cpp)

add_executable(ledsim ${SOURCE_FILES})
target_link_libraries(ledsim ${SDL2_LIBRARIES})
