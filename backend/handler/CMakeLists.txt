include_directories(${CMAKE_CURRENT_SOURCE_DIR})
 
set(DIR_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/handler.cpp)

add_library(libhandler SHARED ${DIR_SRCS})