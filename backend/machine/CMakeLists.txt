include_directories(${CMAKE_CURRENT_SOURCE_DIR})
 
set(DIR_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/machine.cpp)

add_library(machine SHARED ${DIR_SRCS})