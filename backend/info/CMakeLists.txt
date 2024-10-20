include_directories(${CMAKE_CURRENT_SOURCE_DIR})
 
set(DIR_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/cpu.c)

add_library(libcpu SHARED ${DIR_SRCS})