set(DIR_SRCS handler.cpp router.cpp)

add_library(http STATIC ${DIR_SRCS})

target_link_libraries(http machine led)
