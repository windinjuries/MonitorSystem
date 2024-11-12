include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    %{PROJECT_SOURCE_DIR}/easyloggingpp)

 
set(DIR_SRCS led.cpp)

add_library(led SHARED ${DIR_SRCS})