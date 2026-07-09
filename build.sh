#!/bin/bash
# SGIR 构建脚本 (Linux/macOS)
# 用法: ./build.sh [config]
#   config: Debug (默认) 或 Release

set -e  # 遇到错误立即退出

# 解析参数
CONFIG="${1:-Debug}"

# 验证配置
if [[ "$CONFIG" != "Debug" && "$CONFIG" != "Release" ]]; then
    echo "错误: 无效的配置 '$CONFIG'"
    echo "用法: ./build.sh [Debug|Release]"
    exit 1
fi

echo ""
echo "========================================"
echo "SGIR 构建脚本 (Linux/macOS)"
echo "========================================"
echo "配置: $CONFIG"
echo "========================================"
echo ""

# 步骤 1: CMake 配置
echo "[1/2] 正在配置项目..."
cmake --preset local-linux -DSGIR_BUILD_PLUGINS=ON -DSGIR_BUILD_TESTS=ON

echo ""
echo "[1/2] CMake 配置成功"
echo ""

# 步骤 2: 构建项目
echo "[2/2] 正在构建项目 ($CONFIG)..."
cmake --build build/local-linux --config "$CONFIG"

echo ""
echo "========================================"
echo "构建成功!"
echo "========================================"
echo "输出目录: build/local-linux/bin/$CONFIG"
echo "插件目录: build/local-linux/bin/$CONFIG/sgir-plugins-*"
echo "========================================"
echo ""
