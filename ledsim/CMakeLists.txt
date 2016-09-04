cmake_minimum_required(VERSION 2.8)

set (CMAKE_CXX_STANDARD 11)

project(ledsim)

option(DEBUG_OUTPUT "Enable debug output (slow!)" OFF)
#To build a release use:
# cmake -DCMAKE_BUILD_TYPE=Release .
SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Specifies wether or not create a Debug build")


if(DEBUG_OUTPUT)
  add_definitions(-DDEBUG_OUTPUT)
endif()

find_package(SDL2 REQUIRED)

#work around for bug: https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=822210
string(STRIP "${SDL2_LIBRARIES}" stripped_SDL2_LIBRARIES)

include_directories(${SDL2_INCLUDE_DIRS} src lib)


set(SOURCE_FILES src/main.cpp)

add_executable(ledsim ${SOURCE_FILES})
target_link_libraries(ledsim ${stripped_SDL2_LIBRARIES})
