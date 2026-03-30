# cckit

**cckit** 是一个模块化的 C++ 通用工具库，为常规应用、图形学、参数化建模和几何计算等领域的程序，提供核心功能。其旨在替代传统的单一的基础库、通用库，提供更便捷的使用方式、ABI 稳定性和跨语言支持。

> “cc”来源于 C++ 源代码文件常用的后缀名之一“*.cc”，而“kit”则说明这是一个工具性质的通用的库。

**cckit** 遵循四个核心设计原则：

- **ABI 稳定性**：纯 C 接口和 POD 结构确保跨编译器兼容性
- **零开销抽象**：C++ 包装层便于 C++ 调用，且无性能损耗
- **后端可插拔**：支持多种实现（GLM、Eigen 等）
- **语言无关**：可被 Python、C#、Rust 等其他语言调用

## 1. 特性

- **数学库**：完整的向量、矩阵和四元数实现，支持模板化
- **字符串工具**：高级编码转换（UTF-8、GBK、GB2312、GB18030）和宽字符支持
- **文件系统工具**：跨平台的文件和目录操作
- **日志系统**：灵活的日志记录，支持回调、源码位置跟踪和编译期级别控制
- **UUID 生成**：符合 RFC 4122 的 UUID v4 生成
- **Runtime 库**：提供获取系统软硬件、进程信息，以及可执行文件和动态库的路径等功能
- **Random 库**：随机数库
- **geo 库**：专用于地理、gis等领域计算的库
- *（更多特性，敬请期待……）*

## 2. 基本使用示例

```cpp
#include <cckit/math/Math.hpp>
#include <cckit/logging/logging.hpp>

// 向量运算
cckit::math::Vec3f v1(1.0f, 2.0f, 3.0f);
cckit::math::Vec3f v2(4.0f, 5.0f, 6.0f);
cckit::math::Vec3f sum = v1 + v2;
float dot = v1 * v2;  // 点积运算符

// 矩阵运算
Mat4f transform = Mat4f::fromTranslation(cckit::math::Vec3f(1.0f, 2.0f, 3.0f));
cckit::math::Vec3f transformed = transform * cckit::math::Vec3f(0.0f, 0.0f, 0.0f);

// 日志记录
initializeLogging();
CCKIT_LOG_INFO("变换应用成功");
LOG_INFO("变换应用成功");   // 当没有和当前项目中的宏冲突时，可以使用简洁版本的日志宏
```

## 3. 编译和测试

### 3.1 编译前准备

#### 依赖项

- **数学模块**：GLM
- **日志模块**：spdlog
- **UUID 模块**：Boost.UUID
- **字符串模块**：iconv（仅 Linux/Mac）
- **文件系统模块**：ghc-filesystem（可选，用作 std::filesystem 的替代）

#### 编译选项

如下是 **cckit** 项目可用的编译选项，你可以通过“-D” + 选项 + “=ON”，来启用选项。

```cmake
# 构建共享库
option(CCKIT_BUILD_SHARED "构建共享库（暂时不支持启用！）" OFF)

# 构建测试
option(CCKIT_BUILD_TESTS "构建测试" OFF)

# 构建示例
option(CCKIT_BUILD_EXAMPLES "构建示例" OFF)

# 模块特定选项
option(CCKIT_BUILD_STRING "构建字符串模块" ON)
option(CCKIT_BUILD_FS "构建文件系统模块" ON)
option(CCKIT_BUILD_MATH "构建数学模块" ON)
option(CCKIT_BUILD_LOGGING "构建日志模块" ON)
option(CCKIT_BUILD_UUID "构建 UUID 模块" ON)

# FS 子模块特定选项
option(CCKIT_USE_GHC_FS "使用 ghc::filesystem 文件系统库" ON)
```

#### 精度配置

使用数学库时，为了使用不同的浮点数精度，你可以修改根目录下的“CMakeLists.txt”文件中的内容：

```cmake
# 使用 float 精度（默认）
add_definitions(-DCCKIT_MATH_PRECISION_TYPE=float)

# 使用 double 精度
add_definitions(-DCCKIT_MATH_PRECISION_TYPE=double)
```

### 3.2 编译

- 推荐使用 CMake 工具来生成C++项目工程及解决方案 

```bash
cmake --preset local-windows -DCCKIT_BUILD_ALL=ON -DCCKIT_BUILD_TESTS=ON
cmake --build build/local-windows --config Debug
```

### 3.3 运行测试

- 推荐使用 使用 CMake CTest 进行进批量话测试。这需要先进入解决方案所在的根目录，然后执行如下命令。- 或者，你可以进入“build/local-windows/bin/Debug”目录，依次执行各个“test_*.exe”测试程序。

```bash
cd build/local-windows
ctest -C Debug --verbose
```

## 4. 安装和使用

### CMake 集成

如果你的工程使用了 CMake 作为配置工具（**推荐**），则可以直接使用如下配置，来将 cckit 集成到你的项目中。

```cmake
find_package(cckit REQUIRED)

# 链接所有模块
target_link_libraries(my_app PRIVATE cckit::cckit)

# 或链接特定模块
target_link_libraries(my_app PRIVATE 
    cckit::math 
    cckit::logging
)
```

### 使用 vcpkg 安装

**注**：未来会支持！目前，暂不可用，还未推送至vcpkg仓库！

```bash
vcpkg install cckit
```

## 5. 相关文档

- [基本使用文档](docs/BasicUsage.md) - 详细的功能文档
- [设计文档](docs/Design.md) - 架构和设计决策
- [API 参考](docs/Api.md) - 全面的 API 文档

## 6. 贡献

欢迎贡献！请随时提交问题和拉取请求。

## 7. 许可证

MIT License

## 8. 致谢

- 使用 [ghc-filesystem](https://github.com/gulrak/filesystem) 作为另一种文件系统后端
- 使用 [glm](https://glm.g-truc.net) 作为数学后端兼容层
- 使用 [spdlog](https://github.com/gabime/spdlog) 作为日志后端
- 使用 [Boost.Uuid](https://www.boost.org/library/latest/uuid/) 进行 UUID 生成
