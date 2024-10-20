cmake_minimum_required(VERSION 3.1)

project(helloworld)
# cpp 
set(CMAKE_CXX_STANDARD 11)

# c 
set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.cpp)

add_subdirectory(info)
add_subdirectory(handler)

find_package(oatpp REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} libcpu libhandler oatpp::oatpp)