# cckit::runtime

cckit::runtime 是一个跨平台的运行时工具库，提供了获取可执行文件路径、动态库路径、进程信息、环境变量、工作目录、标准路径、系统信息和时间信息等功能。

## 功能特性

### 可执行文件路径
- `getExecutablePath()` - 获取当前可执行文件的完整路径
- `getExecutableDir()` - 获取当前可执行文件所在的目录

### 动态库路径
- `getLibraryPath()` - 获取当前动态库的完整路径（包括文件名）
- `getLibraryDir()` - 获取当前动态库所在的目录路径

### 进程信息
- `getProcessId()` - 获取当前进程 ID
- `getParentProcessId()` - 获取父进程 ID
- `getProcessName()` - 获取当前进程名称
- `getArgc()` - 获取命令行参数数量
- `getArgv()` - 获取命令行参数数组
- `getCommandLine()` - 获取完整的命令行字符串

### 环境变量
- `getEnv()` - 获取环境变量
- `getEnvOrDefault()` - 获取环境变量，如果不存在则返回默认值
- `setEnv()` - 设置环境变量
- `unsetEnv()` - 删除环境变量
- `getPathEnv()` - 获取 PATH 环境变量中的路径列表
- `getAllEnv()` - 获取所有环境变量

### 工作目录
- `getCurrentWorkingDirectory()` - 获取当前工作目录
- `setCurrentWorkingDirectory()` - 设置当前工作目录
- `getTempDirectory()` - 获取临时目录
- `createTempDirectory()` - 创建临时目录
- `createTempFile()` - 创建临时文件

### 标准路径
- `getHomeDirectory()` - 获取用户主目录
- `getUserConfigDirectory()` - 获取用户配置目录
- `getUserDataDirectory()` - 获取用户数据目录
- `getUserCacheDirectory()` - 获取用户缓存目录
- `getAppConfigDirectory()` - 获取应用程序配置目录
- `getAppDataDirectory()` - 获取应用程序数据目录
- `getAppCacheDirectory()` - 获取应用程序缓存目录

### 系统信息
- `getOSName()` - 获取操作系统名称
- `getOSVersion()` - 获取操作系统版本
- `getOSArchitecture()` - 获取系统架构
- `getHostName()` - 获取主机名
- `getUserName()` - 获取当前用户名
- `getProcessorCount()` - 获取处理器核心数
- `getTotalMemory()` - 获取总内存大小（字节）
- `getAvailableMemory()` - 获取可用内存大小（字节）

### 时间信息
- `getProcessUptime()` - 获取进程运行时间（毫秒）
- `getSystemTime()` - 获取系统时间（Unix 时间戳，毫秒）
- `getMonotonicTime()` - 获取单调时间（毫秒）

## 平台支持

- Windows (x86, x64, ARM64)
- Linux (x86, x64, ARM, ARM64)
- macOS (x64, ARM64)

## 使用方法

### C++ 接口

```cpp
#include <cckit/runtime/runtime.hpp>
#include <iostream>

int main()
{
    // 获取可执行文件路径
    std::string exePath = cckit::runtime::getExecutablePath();
    std::cout << "Executable: " << exePath << std::endl;

    // 获取可执行文件目录
    std::string exeDir = cckit::runtime::getExecutableDir();
    std::cout << "Executable Dir: " << exeDir << std::endl;

    // 获取动态库路径
    std::string libPath = cckit::runtime::getLibraryPath();
    std::cout << "Library: " << libPath << std::endl;

    // 获取进程信息
    std::cout << "PID: " << cckit::runtime::getProcessId() << std::endl;
    std::cout << "Process Name: " << cckit::runtime::getProcessName() << std::endl;

    // 获取环境变量
    std::string path = cckit::runtime::getEnv("PATH");
    std::cout << "PATH: " << path << std::endl;

    // 获取系统信息
    std::cout << "OS: " << cckit::runtime::getOSName() << std::endl;
    std::cout << "OS Version: " << cckit::runtime::getOSVersion() << std::endl;
    std::cout << "Architecture: " << cckit::runtime::getOSArchitecture() << std::endl;
    std::cout << "CPU Cores: " << cckit::runtime::getProcessorCount() << std::endl;
    std::cout << "Total Memory: " << cckit::runtime::getTotalMemory() / (1024 * 1024) << " MB" << std::endl;

    // 获取标准路径
    std::cout << "Home: " << cckit::runtime::getHomeDirectory() << std::endl;
    std::cout << "Config: " << cckit::runtime::getUserConfigDirectory() << std::endl;
    std::cout << "Data: " << cckit::runtime::getUserDataDirectory() << std::endl;
    std::cout << "Cache: " << cckit::runtime::getUserCacheDirectory() << std::endl;

    return 0;
}
```

### C 接口

```c
#include <cckit/runtime/runtime.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    // 获取可执行文件路径
    char* exePath = cckit_runtime_get_executable_path();
    if (exePath)
    {
        printf("Executable: %s\n", exePath);
        cckit_runtime_free_string(exePath);
    }

    // 获取进程 ID
    int pid = cckit_runtime_get_process_id();
    printf("PID: %d\n", pid);

    // 获取操作系统名称
    char* osName = cckit_runtime_get_os_name();
    if (osName)
    {
        printf("OS: %s\n", osName);
        cckit_runtime_free_string(osName);
    }

    return 0;
}
```

## CMake 集成

### 使用 find_package

```cmake
find_package(cckit-runtime REQUIRED)

target_link_libraries(your_target PRIVATE cckit::runtime)
```

### 直接添加子目录

```cmake
add_subdirectory(extern/cckit/libs/runtime)

target_link_libraries(your_target PRIVATE cckit::runtime)
```

## 编译选项

- `BUILD_RUNTIME_TESTS` - 构建测试（默认：OFF）
- `BUILD_RUNTIME_EXAMPLES` - 构建示例（默认：OFF）

## 依赖

- C++17 或更高版本
- CMake 3.15 或更高版本

### 平台特定依赖

- Windows: `userenv.lib`, `shell32.lib`, `psapi.lib`
- Linux: 无额外依赖
- macOS: 无额外依赖

## 注意事项

1. **路径长度限制**：Windows 平台默认使用 `MAX_PATH` (260 字符) 缓冲区，如果路径超过此长度，会尝试动态分配更大的缓冲区。

2. **Unicode 支持**：Windows 平台使用 Unicode API (W 版本)，所有字符串都转换为 UTF-8 编码。

3. **动态库路径**：`getLibraryPath()` 函数通过 `dladdr` (Linux/macOS) 或 `GetModuleHandleExW` (Windows) 获取当前动态库的路径。在某些情况下（如静态链接），此函数可能返回空字符串。

4. **命令行参数**：`getArgc()` 和 `getArgv()` 函数在 C++ 中无法直接访问 `main` 函数的参数，因此返回默认值。如果需要访问命令行参数，请在 `main` 函数中保存它们。

5. **内存管理**：C 接口返回的字符串需要使用 `cckit_runtime_free_string()` 释放。

## 许可证

本项目遵循 cckit 项目的许可证。

## 贡献

欢迎提交 Issue 和 Pull Request！

## 更新日志

### 0.1.0 (2026-03-27)
- 初始版本
- 实现基本的运行时工具功能
- 支持 Windows、Linux、macOS 平台
- 提供 C 和 C++ 接口
