#!/bin/bash
# SGIR 清理脚本 (Linux/macOS)
# 用法: ./clean.sh

echo ""
echo "========================================"
echo "SGIR 清理脚本 (Linux/macOS)"
echo "========================================"
echo ""

# 清理构建目录
if [ -d "build/local-linux" ]; then
    echo "正在清理构建目录..."
    rm -rf build/local-linux
    echo "构建目录已清理"
else
    echo "构建目录不存在，无需清理"
fi

echo ""
echo "========================================"
echo "清理完成!"
echo "========================================"
echo ""
