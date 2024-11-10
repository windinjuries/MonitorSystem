include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(DIR_SRCS easylogging++.cc)

add_library(easyloggingpp SHARED ${DIR_SRCS})