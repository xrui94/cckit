# cckit::runtime 测试

本目录包含 cckit::runtime 库的完整测试套件。

## 测试框架

使用 Google Test (GTest) 作为测试框架。如果系统中未安装 GTest，CMake 会自动下载并构建。

## 测试文件

- `test_main.cpp` - 测试主入口
- `test_path.cpp` - 路径相关测试（可执行文件路径、动态库路径）
- `test_process.cpp` - 进程相关测试（进程 ID、进程名称、命令行）
- `test_environment.cpp` - 环境变量测试（获取、设置、删除环境变量）
- `test_directory.cpp` - 目录相关测试（工作目录、临时目录、临时文件）
- `test_system.cpp` - 系统信息测试（操作系统、硬件信息）
- `test_time.cpp` - 时间相关测试（进程运行时间、系统时间、单调时间）

## 构建测试

### 使用 CMake

```bash
# 在项目根目录
mkdir build
cd build

# 配置并启用测试
cmake .. -DBUILD_RUNTIME_TESTS=ON

# 构建
cmake --build . --config Release
```

### 使用 vcpkg

如果使用 vcpkg 管理依赖：

```bash
# 配置时启用测试
cmake .. -DBUILD_RUNTIME_TESTS=ON
```

## 运行测试

### 运行所有测试

```bash
# Linux/macOS
./runtime_tests

# Windows (Debug)
runtime_tests.exe

# Windows (Release)
Release\runtime_tests.exe
```

### 使用 CTest

```bash
# 在 build 目录
ctest --output-on-failure
```

### 运行特定测试

```bash
# 运行特定测试套件
./runtime_tests --gtest_filter=PathTest.*

# 运行特定测试用例
./runtime_tests --gtest_filter=PathTest.GetExecutablePath

# 运行多个测试
./runtime_tests --gtest_filter=PathTest.*:ProcessTest.*
```

## 测试覆盖范围

### 路径测试 (test_path.cpp)
- ✅ 获取可执行文件路径
- ✅ 获取可执行文件目录
- ✅ 获取动态库路径
- ✅ 获取动态库目录
- ✅ 路径一致性验证
- ✅ 路径格式验证

### 进程测试 (test_process.cpp)
- ✅ 获取进程 ID
- ✅ 获取父进程 ID
- ✅ 获取进程名称
- ✅ 获取命令行参数
- ✅ 获取命令行字符串
- ✅ 进程信息一致性
- ✅ 进程稳定性

### 环境变量测试 (test_environment.cpp)
- ✅ 获取环境变量
- ✅ 获取环境变量（带默认值）
- ✅ 设置环境变量
- ✅ 删除环境变量
- ✅ 获取 PATH 环境变量
- ✅ 获取所有环境变量
- ✅ 环境变量操作顺序
- ✅ 环境变量并发访问

### 目录测试 (test_directory.cpp)
- ✅ 获取当前工作目录
- ✅ 设置工作目录
- ✅ 获取临时目录
- ✅ 创建临时目录
- ✅ 创建临时文件
- ✅ 目录操作一致性
- ✅ 目录路径格式

### 系统信息测试 (test_system.cpp)
- ✅ 获取操作系统名称
- ✅ 获取操作系统版本
- ✅ 获取系统架构
- ✅ 获取主机名
- ✅ 获取用户名
- ✅ 获取处理器核心数
- ✅ 获取总内存
- ✅ 获取可用内存
- ✅ 系统信息一致性
- ✅ 系统信息稳定性
- ✅ 平台特定测试

### 时间测试 (test_time.cpp)
- ✅ 获取进程运行时间
- ✅ 获取系统时间
- ✅ 获取单调时间
- ✅ 时间单调性验证
- ✅ 时间一致性验证
- ✅ 时间精度测试
- ✅ 时间稳定性测试
- ✅ 时间性能测试

## 测试统计

- **测试套件**: 6 个
- **测试用例**: 约 100+ 个
- **代码覆盖率**: 目标 > 90%

## 注意事项

1. **平台差异**: 某些测试在不同平台上的行为可能不同，测试代码已考虑这些差异。

2. **环境依赖**: 某些测试依赖于系统环境（如 PATH 环境变量），在受限环境中可能失败。

3. **时间精度**: 时间相关测试的精度受系统调度影响，可能存在轻微偏差。

4. **临时文件**: 测试会创建临时文件和目录，测试完成后会自动清理。

5. **权限要求**: 某些测试可能需要适当的文件系统权限。

## 故障排除

### 测试失败

如果测试失败，可以：

1. 查看详细输出：
   ```bash
   ./runtime_tests --gtest_print_time=1
   ```

2. 运行特定失败的测试：
   ```bash
   ./runtime_tests --gtest_filter=FailedTest.*
   ```

3. 启用详细日志：
   ```bash
   ./runtime_tests --gtest_verbose
   ```

### 编译错误

如果遇到编译错误：

1. 确保 C++17 或更高版本可用
2. 检查是否正确安装了所有依赖
3. 清理构建目录后重新构建：
   ```bash
   rm -rf build
   mkdir build
   cd build
   cmake .. -DBUILD_RUNTIME_TESTS=ON
   ```

## 贡献

添加新测试时，请遵循以下准则：

1. 使用描述性的测试名称
2. 每个测试用例只测试一个功能点
3. 使用 `EXPECT_*` 而非 `ASSERT_*`（除非必须）
4. 在测试失败时提供清晰的错误信息
5. 清理测试创建的资源
6. 更新本 README 文档

## 许可证

与 cckit::runtime 库相同的许可证。
