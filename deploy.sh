#!/bin/bash
# 前端打包和部署脚本

set -e

PROJECT_ROOT="/home/book/Project/MonitorSystem"
FRONTEND_DIR="$PROJECT_ROOT/frontend"
BACKEND_DIR="$PROJECT_ROOT/backend"
echo "=== 项目开始打包和部署 ==="

# 1. 打包前端
echo "[1/3] 打包前端程序..."
cd "$FRONTEND_DIR"
npm run build

if [ ! -d "dist" ]; then
    echo "错误：前端打包失败，dist 目录不存在"
    exit 1
fi

echo "✓ 前端打包完成"


# 2. 编译后端（根据项目构建方式调整）
echo "[2/3] 编译后端程序..."
cd "$BACKEND_DIR"
bash "$BACKEND_DIR/build.sh"
echo "✓ 后端编译完成"


echo "=== 前后端部署 ==="
BUILD_OK=1
DEPLOY=1                # 1 = deploy, 0 = skip deploy 
REMOTE_USER="root"     # 目标机器用户名
REMOTE_HOST="192.168.2.10"  # 目标机器地址
REMOTE_PATH="/root"    # 目标路径
ARTIFACT_PATH="${PROJECT_ROOT}/backend/build/main/MonitorSystem"

# 创建临时部署目录
echo "[3/3] 准备部署文件..."
cd "$PROJECT_ROOT"
rm -rf tmp_deploy
mkdir -p tmp_deploy
cp "$ARTIFACT_PATH" tmp_deploy/
cp -r "$FRONTEND_DIR/dist" tmp_deploy/frontend
echo "✓ 部署文件准备完成"

# 部署到远程服务器
echo ""
echo "=== 开始远程部署 ==="
BUILD_OK=1
DEPLOY=1                # 1 = deploy, 0 = skip deploy
REMOTE_USER="root"     # 目标机器用户名
REMOTE_HOST="192.168.2.10"  # 目标机器地址
REMOTE_PATH="/root/MonitorSystem"    # 目标路径
TMP_DEPLOY_DIR="tmp_deploy"

if [ ${BUILD_OK} -eq 1 ]; then
    if [ "${DEPLOY}" = "1" ] || [ "${DEPLOY}" = "true" ]; then
        if [ -d "${TMP_DEPLOY_DIR}" ]; then
            echo "部署目录 ${TMP_DEPLOY_DIR} -> ${REMOTE_USER}@${REMOTE_HOST}:${REMOTE_PATH}"

            # 在远程机器上清理旧目录并重新创建
            # ssh -o ConnectTimeout=10 "${REMOTE_USER}@${REMOTE_HOST}" "rm -rf ${REMOTE_PATH} && mkdir -p ${REMOTE_PATH}"

            # 传输整个 tmp_deploy 目录内容
            scp -o ConnectTimeout=10 -r "${TMP_DEPLOY_DIR}"/* "${REMOTE_USER}@${REMOTE_HOST}:${REMOTE_PATH}"
            SCP_EXIT_CODE=$?

            if [ ${SCP_EXIT_CODE} -ne 0 ]; then
                echo "scp failed with exit code ${SCP_EXIT_CODE}"
                rm -rf "${TMP_DEPLOY_DIR}"
                exit ${SCP_EXIT_CODE}
            fi

            echo "✓ 部署成功"

            # 清理本地临时目录
            rm -rf "${TMP_DEPLOY_DIR}"
            echo "✓ 已清理本地临时文件"
        else
            echo "部署目录不存在: ${TMP_DEPLOY_DIR}; 跳过部署"
            exit 1
        fi
    else
        echo "DEPLOY 设置为 ${DEPLOY}; 跳过部署步骤"
    fi
else
    echo "构建未成功; 跳过部署"
    exit 1
fi

echo ""
echo "=== 部署完成 ==="