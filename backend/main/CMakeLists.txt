set(SRCS_DIR main.cpp mqtt_user.c)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS_DIR})

target_link_libraries(${CMAKE_PROJECT_NAME} 
                        led 
                        machine 
                        http 
                        libhv
                        flashdb
                        fal
                        sfud
                        spi_modbus
                        # mqtt
                        )
# add_dependencies(${CMAKE_PROJECT_NAME} GenerateConfig)