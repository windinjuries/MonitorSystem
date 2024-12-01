#!/bin/bash 
PROJECT_DIR="./" 

BUILD_DIR="${PROJECT_DIR}/build" 
    
if [ ! -d "${BUILD_DIR}" ]; then 
    mkdir -p "${BUILD_DIR}" 
fi 
    
cd "${BUILD_DIR}" 
    
cmake -DCMAKE_VERBOSE_MAKEFILE=ON .. 
    
# 构建项目，显示详细的编译命令 
cmake --build .  -v

# 检查构建是否成功 
if [ $? -eq 0 ]; then 
    echo "Build succeeded." 
else 
    echo "Build failed." 
    exit 1 
fi 