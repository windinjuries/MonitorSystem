cmake_minimum_required(VERSION 3.1)

project(MonitorSystem)

# set (CMAKE_C_COMPILER "/usr/local/gcc/bin/gcc")
# set (CMAKE_CXX_COMPILER "/usr/local/gcc/bin/g++")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

SET(CMAKE_BUILD_TYPE "Debug")

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

find_package(libhv REQUIRED)


include_directories(machine)
include_directories(http)


add_subdirectory(machine)
add_subdirectory(http)


set(SOURCE_FILES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} machine http libhv.so)