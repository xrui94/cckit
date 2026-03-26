# CCKIT 日志系统使用示例

## 基础使用

### C 接口

```c
#include "cckit/logging/log.h"

int main() {
    // 初始化日志系统
    cckit_log_initialize();

    // 设置回调
    void my_callback(cckit_log_level_t level, const cckit_log_source_loc_t* loc,
                     const char* msg, size_t len, void* context) {
        printf("[%s] %s\n",
               level == CCKIT_LOG_INFO ? "INFO" : "DEBUG",
               msg);
    }

    cckit_log_set_callback(my_callback, nullptr, false);

    // 使用完整宏
    CCKIT_LOG_INFO("Hello, World!");
    CCKIT_LOG_DEBUG("Debug message: {}", 42);

    // 使用简化宏（推荐）
    LOG_INFO("Simplified macro");
    LOG_DEBUG("Debug with value: {}", 100);

    // 清理
    cckit_log_shutdown();
    return 0;
}
```

### C++ 接口

```cpp
#include "cckit/logging/log.hpp"

int main() {
    cckit::initializeLogging();

    // 设置回调
    cckit::setLogCallback(
        [](cckit::LogLevel level, const cckit::SourceLoc& loc,
           const std::string& msg, void* context) {
            std::cout << "[" << static_cast<int>(level) << "] " << msg << std::endl;
        },
        nullptr,
        false
    );

    // 使用完整宏
    CCKIT_LOG_INFO("Hello, World!");
    CCKIT_LOG_DEBUG("Debug message: {}", 42);

    // 使用简化宏（推荐）
    LOG_INFO("Simplified macro");
    LOG_DEBUG("Debug with value: {}", 100);

    cckit::shutdownLogging();
    return 0;
}
```

## 高级使用

### 禁用简化宏

如果与其他库的宏冲突，可以禁用简化宏：

```cpp
// 在包含 log.h 之前定义
#define CCKIT_DISABLE_SIMPLE_MACROS

#include "cckit/logging/log.hpp"

int main() {
    // 只能使用完整宏
    CCKIT_LOG_INFO("Using full macro");
    // LOG_INFO("This will not work"); // 编译错误

    return 0;
}
```

### 编译期日志级别控制

```cpp
// 设置最低日志级别（编译期）
#define CCKIT_MIN_LOG_LEVEL 2  // 只输出 INFO 及以上

#include "cckit/logging/log.hpp"

int main() {
    // 这些调用会被编译器优化掉
    LOG_TRACE("This won't be compiled");
    LOG_DEBUG("This won't be compiled");

    // 这些会被编译
    LOG_INFO("This will be compiled");
    LOG_WARN("This will be compiled");

    return 0;
}
```

### 多线程使用

```cpp
#include "cckit/logging/log.hpp"
#include <thread>
#include <vector>

void worker_thread(int id) {
    for (int i = 0; i < 10; ++i) {
        LOG_INFO("Thread {} - Message {}", id, i);
    }
}

int main() {
    cckit::initializeLogging();

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(worker_thread, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    cckit::shutdownLogging();
    return 0;
}
```

### 与用户 spdlog 共存

```cpp
#include "cckit/logging/log.hpp"
#include <spdlog/spdlog.h>

int main() {
    // 用户设置自己的 spdlog
    spdlog::set_level(spdlog::level::debug);
    auto user_logger = spdlog::stdout_logger_mt("user_logger");

    // 使用 cckit 日志（完全隔离）
    cckit::setLogLevel(cckit::LogLevel::trace);
    LOG_INFO("CCKIT log message");

    // 使用自己的 spdlog
    user_logger->info("User log message");

    // 两者互不干扰！
    return 0;
}
```

## 常见场景

### GUI 应用集成

```cpp
#include "cckit/logging/log.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
        // 设置回调，将日志发送到 GUI
        cckit::setLogCallback([this](auto level, auto loc, const auto& msg, auto ctx) {
            // 在主线程中更新 UI
            QMetaObject::invokeMethod(this, [this, msg]() {
                logTextEdit->append(QString::fromStdString(msg));
            });
        });
    }

private:
    QTextEdit* logTextEdit;
};
```

### 文件日志

```cpp
#include "cckit/logging/log.hpp"
#include <fstream>

int main() {
    std::ofstream log_file("app.log", std::ios::app);

    cckit::setLogCallback([&log_file](auto level, auto loc, const auto& msg, auto ctx) {
        log_file << msg << std::endl;
    });

    LOG_INFO("Application started");

    // ... 应用逻辑 ...

    return 0;
}
```

### 网络日志传输

```cpp
#include "cckit/logging/log.hpp"
#include <zmq.hpp>

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PUB);
    socket.connect("tcp://localhost:5555");

    cckit::setLogCallback([&socket](auto level, auto loc, const auto& msg, auto ctx) {
        zmq::message_t message(msg.size());
        memcpy(message.data(), msg.data(), msg.size());
        socket.send(message, zmq::send_flags::dontwait);
    });

    LOG_INFO("Connected to log server");

    return 0;
}
```

## 性能优化

### 延迟初始化

```cpp
#include "cckit/logging/log.hpp"

int main() {
    // 不显式初始化，首次使用时自动初始化
    LOG_INFO("This will trigger lazy initialization");

    return 0;
}
```

### 编译期优化

```cpp
// Release 模式下自动降低日志级别
#ifndef CCKIT_MIN_LOG_LEVEL
    #ifdef NDEBUG
        #define CCKIT_MIN_LOG_LEVEL CCKIT_LOG_LEVEL_INFO
    #else
        #define CCKIT_MIN_LOG_LEVEL CCKIT_LOG_LEVEL_TRACE
    #endif
#endif

#include "cckit/logging/log.hpp"
```

## 故障排除

### 宏冲突

如果遇到宏冲突，有以下解决方案：

1. **禁用简化宏**
```cpp
#define CCKIT_DISABLE_SIMPLE_MACROS
#include "cckit/logging/log.hpp"
```

2. **使用完整宏**
```cpp
#include "cckit/logging/log.hpp"

// 使用完整宏避免冲突
CCKIT_LOG_INFO("Using full macro");
```

3. **调整包含顺序**
```cpp
// 先包含其他库
#include "other_library.h"

// 后包含 cckit
#include "cckit/logging/log.hpp"
```

### 重入日志

如果在回调中记录日志，会自动重定向到 stderr：

```cpp
cckit::setLogCallback([](...) {
    // 这条日志会被重定向到 stderr
    LOG_INFO("This will go to stderr: [CCKIT Recursive] ...");
});
```

## 最佳实践

1. **推荐使用简化宏** - `LOG_INFO()` 比 `CCKIT_LOG_INFO()` 更简洁
2. **避免在回调中记录日志** - 防止重入和死锁
3. **设置合适的日志级别** - 生产环境使用 INFO 或 WARN
4. **使用格式化字符串** - `LOG_INFO("Value: {}", val)` 而非字符串拼接
5. **及时清理** - 程序退出前调用 `cckit::shutdownLogging()`
