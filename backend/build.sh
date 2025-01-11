#!/bin/bash

# ARCH:
# x86_64
# Aarch64 
export ARCH="Aarch64"

# 检查 ARCH 的值是否为 x86 或 arm
if [ "$ARCH" != "x86_64" ] && [ "$ARCH" != "Aarch64" ]
then
    echo "Error: ARCH is not set to 'x86' or 'arm'. Current value: ${ARCH}"
    exit 1
fi

if [ "$ARCH" == "Aarch64" ]
then 
    export TOOLCHAIN_PREFIX="aarch64-linux-gnu-"
fi

PROJECT_DIR="./" 

BUILD_DIR="${PROJECT_DIR}/build" 
    
if [ ! -d "${BUILD_DIR}" ]; then 
    mkdir -p "${BUILD_DIR}" 
fi 
    
cd "${BUILD_DIR}" 
    
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1
    
# 构建项目，显示详细的编译命令 
cmake --build .  -v

# 检查构建是否成功 
if [ $? -eq 0 ]; then 
    echo "Build succeeded." 
else 
    echo "Build failed." 
    exit 1 
fi 