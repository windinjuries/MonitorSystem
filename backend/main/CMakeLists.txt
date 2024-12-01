set(SRCS_DIR main.cpp)

add_executable(${PROJECT_NAME} ${SRCS_DIR})

target_link_libraries(${PROJECT_NAME} 
                        easyloggingpp
                        led 
                        machine 
                        http 
                        libhv.so)