#!/bin/bash

# Simple build + optional deploy script for MonitorSystem backend

# ---------- Configuration (edit these variables) ----------
# ARCH: "x86_64" or "Aarch64"
ARCH="Aarch64"
# Project/build paths
PROJECT_DIR="./"
BUILD_DIR="${PROJECT_DIR}/build"
# Deploy control: 1 = deploy after successful build, 0 = skip deploy
DEPLOY=1
# Remote deploy target
REMOTE_USER="root"
REMOTE_HOST="192.168.2.17"
REMOTE_PATH="/root"
# Artifact relative to build dir
ARTIFACT_PATH="${BUILD_DIR}/main/MonitorSystem"
# ---------------------------------------------------------

# 检查 ARCH 的值
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

# 返回项目根目录
cd .. || { echo "Failed to change directory to project root."; exit 1; }

# 部署开关及配置（不使用环境变量）
# 修改下面的变量以控制是否部署及部署目标
DEPLOY=1                # 1 = deploy, 0 = skip deploy
REMOTE_USER="root"     # 目标机器用户名
REMOTE_HOST="192.168.2.17"  # 目标机器地址
REMOTE_PATH="/root"    # 目标路径
ARTIFACT_PATH="${BUILD_DIR}/main/MonitorSystem"

if [ ${BUILD_OK} -eq 1 ]; then
    if [ "${DEPLOY}" = "1" ] || [ "${DEPLOY}" = "true" ]; then
        if [ -f "${ARTIFACT_PATH}" ]; then
            echo "Deploying artifact ${ARTIFACT_PATH} -> ${REMOTE_USER}@${REMOTE_HOST}:${REMOTE_PATH}"
            scp -o ConnectTimeout=10 "${ARTIFACT_PATH}" "${REMOTE_USER}@${REMOTE_HOST}:${REMOTE_PATH}"
            SCP_EXIT_CODE=$?
            if [ ${SCP_EXIT_CODE} -ne 0 ]; then
                echo "scp failed with exit code ${SCP_EXIT_CODE}"
                exit ${SCP_EXIT_CODE}
            fi
            echo "scp succeeded; opening ssh session to ${REMOTE_USER}@${REMOTE_HOST}"
            ssh "${REMOTE_USER}@${REMOTE_HOST}"
        else
            echo "Artifact not found: ${ARTIFACT_PATH}; skipping deploy."
            exit 1
        fi
    else
        echo "DEPLOY is set to ${DEPLOY}; skipping deploy step."
    fi
else
    echo "Build did not succeed; skipping deploy."
    exit 1
fi

