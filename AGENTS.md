# AGENTS.md

## 项目概述

cckit 是一个 C++17 模块化通用工具库，使用 CMake + vcpkg 构建，包含 8 个独立模块。

## 构建命令

```powershell
# 配置（Windows，必须指定模块和测试开关）
cmake --preset local-windows -DCCKIT_BUILD_ALL=ON -DCCKIT_BUILD_TESTS=ON

# 编译
cmake --build build/local-windows --config Debug
```

`CCKIT_BUILD_TESTS` 默认 OFF，需要显式开启。单个模块可用 `-DCCKIT_BUILD_<MODULE>=ON` 代替 `CCKIT_BUILD_ALL`。

**注意**：`CMakeUserPresets.json` 中的 `local-windows` 预设包含本地机器硬编码路径（`VCPKG_ROOT=C:/env/vcpkg` 等），在其他机器上需要修改或创建新的 user preset。

## 运行测试

```powershell
cd build/local-windows
ctest -C Debug --verbose
```

也可以直接运行可执行文件：`build/local-windows/bin/Debug/test_*.exe`。测试通过 CTest 注册，每个模块的 `test/CMakeLists.txt` 使用 `add_test()`。

## 模块结构

8 个模块位于 `libs/`，每个模块遵守统一结构：

```
libs/<module>/
├── CMakeLists.txt     # add_library(cckit-<module>)
├── include/cckit/<module>/   # 公开头文件
├── src/               # 实现
└── test/              # 测试（仅当 CCKIT_BUILD_TESTS=ON 时编译）
```

模块目标名：`cckit-<module>`，别名：`cckit::<module>`。顶层 `cckit` 是一个 INTERFACE 目标，聚合所有已启用的模块（alias: `cckit::cckit`）。

## 重要注意事项

- **`CCKIT_BUILD_SHARED` 不支持**：目前标记为 "Not supported yet!"，必须保持 OFF。
- **`build.sh` 和 `clean.sh` 已过时**：它们仍引用旧项目名 "SGIR" 和错误的 preset 名称，不要直接使用。
- **math 模块精度**：通过根 `CMakeLists.txt` 中的 `add_definitions(-DCCKIT_MATH_PRECISION_TYPE=float|double)` 控制，默认 float。
- **fs 模块后端**：默认 `std::filesystem`，可通过 `-DCCKIT_USE_GHC_FS=ON` 切换到 `ghc::filesystem`（用于旧编译器）。
- **无 CI 配置**：仓库中没有 `.github/` 或其他 CI 工作流。

## 依赖（vcpkg）

boost-uuid (>=1.89.0), glm (>=1.0.3), spdlog (>=1.15.3), ghc-filesystem (>=1.5.14)

## 代码风格

- 源文件头文件均使用中文注释。
- 日志宏优先使用 `CCKIT_LOG_*`（带命名空间前缀），仅在确认无冲突时使用短版本 `LOG_*`。
