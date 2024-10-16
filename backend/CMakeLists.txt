cmake_minimum_required(VERSION 3.1)
project(helloworld)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(SOURCE_FILES main.cpp handler.h)

# 查找 oatpp 依赖
find_package(oatpp REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# 将目标文件与库文件进行链接
target_link_libraries(${PROJECT_NAME} oatpp::oatpp)