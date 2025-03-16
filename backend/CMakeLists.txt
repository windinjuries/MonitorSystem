cmake_minimum_required(VERSION 3.16)


message(STATUS "ARCH: $ENV{ARCH}")
if(NOT $ENV{ARCH} EQUAL "x86_64")
    set(CMAKE_C_COMPILER "$ENV{TOOLCHAIN_PREFIX}gcc")
    set(CMAKE_CXX_COMPILER "$ENV{TOOLCHAIN_PREFIX}g++")
endif()

# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -ggdb -pthread")

set(KCONFIG_DIR ${CMAKE_SOURCE_DIR}/kconfig)


set(PROJECT_INCLUDE_DIRS)

project(MonitorSystem VERSION 1.0.0)

# include_directories(easyloggingpp
#                     device/machine
#                     device/led
#                     http
#                     ${PROJECT_INCLUDE_DIRS})


add_subdirectory(thirdparty/libhv)

include_directories(easyloggingpp
                    device/machine
                    device/led
                    device/sfud/inc
                    device/fal/inc
                    device/flashdb/inc
                    http
                    ${PROJECT_INCLUDE_DIRS}
                    ${CMAKE_BINARY_DIR})
add_subdirectory(device)
add_subdirectory(http)
add_subdirectory(main)

message("KCONFIG_DIR: ${KCONFIG_DIR}  ${CMAKE_BINARY_DIR}")
add_custom_target(generate_kconfig_h
    COMMAND python3 ${KCONFIG_DIR}/kconfig.py -g -d ${KCONFIG_DIR}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating kconfig.h"
    DEPENDS ${KCONFIG_DIR}/kconfig
    VERBATIM
)

add_dependencies(http generate_kconfig_h)

# Debug info
function(print_include_dirs)
    get_property(INCLUDE_DIRS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    foreach(DIR ${INCLUDE_DIRS})
        message(STATUS "include dir = ${DIR}")
    endforeach()
endfunction()
print_include_dirs()