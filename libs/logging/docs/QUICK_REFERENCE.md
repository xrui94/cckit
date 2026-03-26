# CCKIT 日志系统快速参考

## 快速开始

### C 接口

```c
#include "cckit/logging/log.h"

int main() {
    cckit_log_initialize();

    // 使用简化宏（推荐）
    LOG_INFO("Hello, World!");
    LOG_DEBUG("Value: {}", 42);

    // 使用完整宏（避免冲突）
    CCKIT_LOG_INFO("Full macro");

    cckit_log_shutdown();
    return 0;
}
```

### C++ 接口

```cpp
#include "cckit/logging/log.hpp"

int main() {
    cckit::initializeLogging();

    // 使用简化宏（推荐）
    LOG_INFO("Hello, World!");
    LOG_DEBUG("Value: {}", 42);

    // 使用完整宏（避免冲突）
    CCKIT_LOG_INFO("Full macro");

    cckit::shutdownLogging();
    return 0;
}
```

## 宏对比

| 简化宏 | 完整宏 | 说明 |
|--------|--------|------|
| `LOG_TRACE()` | `CCKIT_LOG_TRACE()` | 最详细的日志 |
| `LOG_DEBUG()` | `CCKIT_LOG_DEBUG()` | 调试信息 |
| `LOG_INFO()` | `CCKIT_LOG_INFO()` | 一般信息 |
| `LOG_WARN()` | `CCKIT_LOG_WARN()` | 警告信息 |
| `LOG_ERROR()` | `CCKIT_LOG_ERROR()` | 错误信息 |
| `LOG_CRITICAL()` | `CCKIT_LOG_CRITICAL()` | 严重错误 |

## 常见场景

### 禁用简化宏

如果与其他库冲突：

```cpp
#define CCKIT_DISABLE_SIMPLE_MACROS
#include "cckit/logging/log.hpp"

// 只能使用完整宏
CCKIT_LOG_INFO("Message");
```

### 编译期日志级别控制

```cpp
// 设置最低日志级别
#define CCKIT_MIN_LOG_LEVEL 2  // 只输出 INFO 及以上

#include "cckit/logging/log.hpp"

LOG_TRACE("This won't be compiled");  // 被优化掉
LOG_INFO("This will be compiled");    // 会被编译
```

### 格式化日志

```cpp
LOG_INFO("User {} logged in from {}", username, ip);
LOG_DEBUG("Processing {} items, total size: {} bytes", count, size);
LOG_ERROR("Failed to open file {}: {}", filename, error_message);
```

### 多线程安全

```cpp
#include <thread>
#include <vector>

void worker(int id) {
    LOG_INFO("Worker {} started", id);
    // ... 工作逻辑 ...
    LOG_INFO("Worker {} finished", id);
}

int main() {
    cckit::initializeLogging();

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    cckit::shutdownLogging();
    return 0;
}
```

## 重要提醒

### ⚠️ 不要在回调中记录日志

```cpp
// ❌ 错误：在回调中记录日志
cckit::setLogCallback([](...) {
    LOG_INFO("This will go to stderr: [CCKIT Recursive] ...");
});

// ✅ 正确：回调中只做转发
cckit::setLogCallback([](...) {
    send_to_gui(msg);
});
```

### 🔒 与用户 spdlog 完全隔离

```cpp
#include <spdlog/spdlog.h>
#include "cckit/logging/log.hpp"

int main() {
    // 用户的 spdlog
    spdlog::set_level(spdlog::level::debug);
    SPDLOG_INFO("User log");

    // cckit 的日志（完全独立）
    cckit::setLogLevel(cckit::LogLevel::trace);
    LOG_INFO("CCKIT log");

    // 两者互不干扰！
    return 0;
}
```

## 性能提示

### 延迟初始化

```cpp
// 不需要显式初始化，首次使用时自动初始化
LOG_INFO("This triggers lazy initialization");
```

### 编译期优化

```cpp
// Release 模式下自动降低日志级别
LOG_TRACE("This will be optimized out in Release");
LOG_INFO("This will be compiled in Release");
```

## 故障排除

### 宏冲突

**问题**：编译错误，提示宏重定义

**解决方案**：
```cpp
#define CCKIT_DISABLE_SIMPLE_MACROS
#include "cckit/logging/log.hpp"

// 使用完整宏
CCKIT_LOG_INFO("Message");
```

### 重入日志

**问题**：回调中的日志输出到 stderr

**原因**：这是正常的死锁防护机制

**解决方案**：不要在回调中记录日志

### 日志不显示

**问题**：日志没有输出

**检查清单**：
1. 是否调用了 `cckit_log_initialize()` 或设置了回调？
2. 日志级别是否设置正确？
3. 编译期日志级别是否过滤了日志？

## 最佳实践

1. ✅ **使用简化宏** - `LOG_INFO()` 更简洁
2. ✅ **设置合适的日志级别** - 生产环境用 INFO/WARN
3. ✅ **使用格式化字符串** - `LOG_INFO("Value: {}", val)`
4. ✅ **避免在回调中记录日志** - 防止重入
5. ✅ **及时清理** - 程序退出前调用 `cckit::shutdownLogging()`

## 更多信息

- 详细文档：[README.md](README.md)
- 使用示例：[EXAMPLES.md](EXAMPLES.md)
- API 参考：查看头文件中的详细注释
