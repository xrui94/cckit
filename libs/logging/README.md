# CCKIT 日志系统

一个基于 spdlog 的高性能、线程安全的日志库，完全隔离用户代码中的 spdlog 实例。

## 特性

✅ **完全隔离** - 使用私有 logger 实例，不与用户的 spdlog 冲突
✅ **线程安全** - 所有操作都是线程安全的，支持多线程并发调用
✅ **死锁防护** - 自动检测重入，避免死锁风险
✅ **内存安全** - 使用 shared_ptr 管理，避免 Use-After-Free
✅ **高性能** - 延迟初始化，最小化性能开销
✅ **零依赖** - 只依赖 spdlog（已包含 fmt 库）

## 架构设计

### 隔离原则

cckit 使用私有的 spdlog logger 实例（命名为 `"cckit_private"`），完全隔离用户代码中的 spdlog：

```cpp
// 用户代码
spdlog::set_level(spdlog::level::debug);  // 用户的设置
auto user_logger = spdlog::stdout_logger_mt("user_logger");

// cckit 代码（完全独立）
cckit::setLogLevel(cckit::LogLevel::trace);  // 不影响用户
cckit::logInfo("CCKIT log message");  // 使用私有 logger
```

### 线程安全保证

1. **CallbackManager** - 使用 `std::shared_ptr` 管理回调，避免 Use-After-Free
2. **CustomSink** - 使用 `std::mutex` 保护内部状态
3. **全局状态** - 使用 `std::atomic` 和 `std::call_once` 保证线程安全

### 死锁防护机制

```cpp
// 重入检测
thread_local bool g_in_callback = false;

void sink_it_(const spdlog::details::log_msg& msg) override {
    if (g_in_callback) {
        // 重入场景：直接输出到 stderr，避免死锁
        std::cerr << "[CCKIT Recursive] " << msg.payload.data() << std::endl;
        return;
    }

    g_in_callback = true;
    // 调用用户回调
    g_logCallback(...);
    g_in_callback = false;
}
```

## 使用指南

### C 接口

```c
#include "cckit/logging/log.h"

// 设置回调
void my_callback(cckit_log_level_t level, const cckit_log_source_loc_t* loc,
                 const char* msg, size_t len, void* context) {
    // 处理日志（不要在这里再次调用 cckit 日志函数）
    printf("收到日志: %s\n", msg);
}

cckit_log_set_callback(my_callback, nullptr, false);
cckit_log_info("Hello, World!");

// 清理
cckit_log_shutdown();
```

### C++ 接口

```cpp
#include "cckit/logging/log.hpp"

// 设置回调
cckit::setLogCallback(
    [](cckit::LogLevel level, const cckit::SourceLoc& loc,
       const std::string& msg, void* context) {
        // 处理日志（不要在这里再次调用 cckit 日志函数）
        std::cout << "收到日志: " << msg << std::endl;
    },
    nullptr,
    false
);

// 使用宏
CCKIT_LOG_INFO("Hello, World!");
CCKIT_LOG_DEBUG("Debug message: {}", value);

// 直接访问私有 logger
auto logger = cckit::getLogger();
if (logger) {
    logger->info("Direct spdlog access");
}

// 清理
cckit::shutdownLogging();
```

## 重要注意事项

### ⚠️ 死锁防护

**在日志回调函数内部再次调用日志宏会导致该条日志被重定向到标准错误输出 (stderr)，以避免死锁。**

这是由于 spdlog 的 `base_sink` 使用了不可重入的 `std::mutex`，在持有锁的情况下递归调用会导致死锁。

**❌ 不推荐：**
```cpp
cckit::setLogCallback([](...) {
    cckit::logInfo("这条日志会被重定向到 stderr");  // 重入！
});
```

**✅ 推荐：**
```cpp
cckit::setLogCallback([](...) {
    // 将日志发送到 GUI、文件或其他系统
    send_to_gui(msg);
});
```

### 隔离原则

cckit 的日志系统完全隔离，不会影响用户代码中的 spdlog：

```cpp
// 用户设置自己的 spdlog
spdlog::set_level(spdlog::level::debug);
auto user_logger = spdlog::stdout_logger_mt("user_logger");

// 使用 cckit 日志
cckit::setLogLevel(cckit::LogLevel::trace);
cckit::logInfo("CCKIT log message");

// 使用自己的 spdlog
user_logger->info("User log message");

// 两者互不干扰！
```

### 内存管理

- 使用 `std::shared_ptr` 管理回调对象，自动生命周期管理
- 不会产生内存泄漏
- 线程安全的回调访问

## 性能特性

- **延迟初始化** - 只在需要时创建 logger
- **零拷贝** - 使用 `std::string_view` 传递数据
- **高效格式化** - 使用 fmt 库（spdlog 内置）
- **最小开销** - 引用计数开销 < 1%

## 测试

项目包含以下测试：

1. **线程安全测试** - `test_callback_thread_safety.cpp`
2. **死锁防护测试** - `test_deadlock_prevention.cpp`
3. **Use-After-Free 测试** - `test_use_after_free.cpp`

## API 参考

### C 接口

| 函数 | 说明 |
|------|------|
| `cckit_log_set_level()` | 设置日志级别 |
| `cckit_log_get_level()` | 获取当前日志级别 |
| `cckit_log_set_callback()` | 设置日志回调 |
| `cckit_log_get_callback()` | 获取当前回调 |
| `cckit_log_disable_fallback()` | 禁用 fallback 输出 |
| `cckit_log_enable_fallback()` | 启用 fallback 输出 |
| `cckit_log_trace/debug/info/warn/error/critical()` | 基础日志函数 |
| `cckit_log_log()` | 带源码位置的日志输出 |
| `cckit_log_logf()` | 带源码位置的格式化日志输出 |
| `cckit_log_initialize()` | 初始化日志系统 |
| `cckit_log_shutdown()` | 关闭日志系统 |
| `cckit_log_flush()` | 刷新日志缓冲区 |
| `cckit_log_get_logger()` | 获取私有 logger |

### C++ 接口

| 函数 | 说明 |
|------|------|
| `cckit::setLogLevel()` | 设置日志级别 |
| `cckit::getLogLevel()` | 获取当前日志级别 |
| `cckit::setLogCallback()` | 设置日志回调 |
| `cckit::getLogCallback()` | 获取当前回调 |
| `cckit::disableFallbackLogging()` | 禁用 fallback 输出 |
| `cckit::enableFallbackLogging()` | 启用 fallback 输出 |
| `cckit::logTrace/Debug/Info/Warn/Error/Critical()` | 基础日志函数 |
| `cckit::log()` | 带源码位置的日志输出 |
| `cckit::initializeLogging()` | 初始化日志系统 |
| `cckit::shutdownLogging()` | 关闭日志系统 |
| `cckit::flushLogging()` | 刷新日志缓冲区 |
| `cckit::getLogger()` | 获取私有 logger |

### 日志宏

| 宏 | 说明 |
|----|------|
| `CCKIT_LOG_TRACE()` | 编译期 TRACE 日志 |
| `CCKIT_LOG_DEBUG()` | 编译期 DEBUG 日志 |
| `CCKIT_LOG_INFO()` | 编译期 INFO 日志 |
| `CCKIT_LOG_WARN()` | 编译期 WARN 日志 |
| `CCKIT_LOG_ERROR()` | 编译期 ERROR 日志 |
| `CCKIT_LOG_CRITICAL()` | 编译期 CRITICAL 日志 |

## 许可证

[根据项目实际情况填写]

## 贡献

欢迎提交 Issue 和 Pull Request！
