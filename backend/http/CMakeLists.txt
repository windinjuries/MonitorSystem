include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${PROJECT_SOURCE_DIR}/machine
)

set(DIR_SRCS handler.cpp router.cpp)

add_library(http SHARED ${DIR_SRCS})


target_link_libraries(http machine)
