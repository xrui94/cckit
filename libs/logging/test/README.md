# CCKIT Logging Tests

本目录包含 cckit 日志系统的测试代码。

## 测试列表

### 1. test_callback_thread_safety.cpp
**目的**：测试回调函数的线程安全性

**测试内容**：
- 多线程并发设置回调
- 内存泄漏检测
- Use-After-Free 防护

**运行方式**：
```bash
cd build
ctest -R callback_thread_safety
# 或直接运行
./test/test_callback_thread_safety.exe
```

### 2. test_deadlock_prevention.cpp
**目的**：测试死锁防护机制

**测试内容**：
- 递归日志调用
- 深层递归测试
- 多线程递归测试

**运行方式**：
```bash
cd build
ctest -R deadlock_prevention
# 或直接运行
./test/test_deadlock_prevention.exe
```

### 3. test_use_after_free.cpp
**目的**：测试 Use-After-Free 防护

**测试内容**：
- 并发设置/清除回调
- 并发回调访问
- 回调生命周期测试

**运行方式**：
```bash
cd build
ctest -R use_after_free
# 或直接运行
./test/test_use_after_free.exe
```

## 编译和运行

### 编译

```bash
# 配置项目
 cmake --preset local-windows -DCCKIT_BUILD_LOGGING=ON -DCCKIT_BUILD_TESTS=ON

# 编译
cmake --build build/local-windows --config Debug
```

### 运行所有测试

```bash
cd build
ctest --output-on-failure
```

### 运行单个测试

```bash
cd build
ctest -R <test_name>
```

### 运行测试并显示详细输出

```bash
cd build
ctest -V --output-on-failure
```

## 测试依赖

- **spdlog** - 日志后端
- **fmt** - 格式化库

## 测试覆盖率

| 测试 | 覆盖功能 | 状态 |
|------|----------|------|
| callback_thread_safety | 线程安全、内存管理 | ✅ |
| deadlock_prevention | 死锁防护、重入检测 | ✅ |
| use_after_free | Use-After-Free 防护 | ✅ |

## 添加新测试

1. 在 `test/` 目录中创建新的测试文件
2. 在 `test/CMakeLists.txt` 中添加新的可执行文件
3. 使用 `add_test()` 注册测试

### 示例

```cmake
# test/CMakeLists.txt
add_executable(test_my_feature
    test_my_feature.cpp
)

target_link_libraries(test_my_feature
    PRIVATE
    spdlog::spdlog
    fmt::fmt
)

add_test(NAME my_feature COMMAND test_my_feature)
```

## 注意事项

### 1. 测试超时

所有测试都有 30 秒的超时限制。如果测试运行时间超过 30 秒，会被标记为失败。

### 2. 内存检测

建议使用 Valgrind 或 AddressSanitizer 进行内存检测：

```bash
# 使用 AddressSanitizer
cmake -B build -DCMAKE_CXX_FLAGS="-fsanitize=address -g" -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# 运行测试
cd build
ctest
```

### 3. 线程检测

建议使用 ThreadSanitizer 进行线程检测：

```bash
# 使用 ThreadSanitizer
cmake -B build -DCMAKE_CXX_FLAGS="-fsanitize=thread -g" -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# 运行测试
cd build
ctest
```

## 故障排除

### 测试失败

如果测试失败，请检查：

1. **依赖版本** - 确保 spdlog 和 fmt 版本正确
2. **编译选项** - 确保使用了正确的 C++ 标准（C++17）
3. **运行环境** - 确保在正确的目录中运行测试

### 内存泄漏

如果发现内存泄漏，请检查：

1. **回调管理** - 确保使用 `std::shared_ptr` 管理回调
2. **资源清理** - 确保调用 `cckit_log_shutdown()`
3. **线程安全** - 确保所有操作都是线程安全的

### 死锁

如果发现死锁，请检查：

1. **重入检测** - 确保启用了重入检测机制
2. **回调逻辑** - 确保不在回调中记录日志
3. **锁顺序** - 确保没有循环等待

## 许可证

与主项目保持一致。
