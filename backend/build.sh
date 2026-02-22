#!/bin/bash


# ARCH: "x86_64" or "Aarch64"
ARCH="Aarch64"
# Project/build paths
PROJECT_DIR="./"
BUILD_DIR="${PROJECT_DIR}/build"


if [ "$ARCH" != "x86_64" ] && [ "$ARCH" != "Aarch64" ]; then
    echo "Error: ARCH is not set to 'x86_64' or 'Aarch64'. Current value: ${ARCH}"
    exit 1
fi
export ARCH=${ARCH} 
if [ "$ARCH" == "Aarch64" ]
then 
    export TOOLCHAIN_PREFIX="aarch64-linux-gnu-"
fi

if [ ! -d "${BUILD_DIR}" ]; then
    mkdir -p "${BUILD_DIR}"
fi

cd "${BUILD_DIR}" || { echo "Failed to enter build directory: ${BUILD_DIR}"; exit 1; }

cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1

# 构建项目，显示详细的编译命令
cmake --build .
BUILD_EXIT_CODE=$?

# 设置构建标志
BUILD_OK=0
if [ ${BUILD_EXIT_CODE} -eq 0 ]; then
    echo "Build succeeded."
    BUILD_OK=1
else
    echo "Build failed. (exit ${BUILD_EXIT_CODE})"
    BUILD_OK=0
fi

