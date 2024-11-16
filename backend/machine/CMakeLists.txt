include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    %{PROJECT_SOURCE_DIR}/easyloggingpp
                    %{PROJECT_SOURCE_DIR}/led)

 
set(DIR_SRCS machine.cpp)

add_library(machine SHARED ${DIR_SRCS})