include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    %{PROJECT_SOURCE_DIR}/easyloggingpp)

 
set(DIR_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/machine.cpp)

add_library(machine SHARED ${DIR_SRCS})