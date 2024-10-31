cmake_minimum_required(VERSION 3.1)

project(helloworld)

# cpp 
# set (CMAKE_C_COMPILER "/usr/local/gcc/bin/gcc")
# set (CMAKE_CXX_COMPILER "/usr/local/gcc/bin/g++")
set(CMAKE_CXX_STANDARD 11)

# c 
set(CMAKE_C_STANDARD 99)


find_package(oatpp REQUIRED)
message(${oatpp_INCLUDE_DIRS})

include_directories(${oatpp_INCLUDE_DIRS})
add_subdirectory(info)
include_directories(info machine handler)

# add_subdirectory(handler)

set(SOURCE_FILES main.cpp handler/handler.hpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} libcpu oatpp::oatpp)