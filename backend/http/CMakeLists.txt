set(DIR_SRCS handler.cpp router.cpp)

add_library(http SHARED ${DIR_SRCS})

target_link_libraries(http easyloggingpp machine led)
