// log.cpp - 日志后端实现
#include "cckit/logging/Logging.hpp"
#include "cckit/export.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/pattern_formatter.h>

#include <iostream>
#include <cstdarg>
#include <cstdio>
#include <mutex>
#include <atomic>
#include <memory>

// ========================================
// 私有命名空间 - 避免与用户代码冲突
// ========================================

namespace {
    // 全局回调函数和上下文
    cckit_log_callback_t g_logCallback = nullptr;
    void* g_logContext = nullptr;

    // 是否禁用 fallback 输出
    std::atomic<bool> g_fallbackDisabled{ false };

    // 用于只显示一次警告
    std::atomic<bool> g_warningShown{ false };

    // fallback logger（静态，全局唯一）
    std::shared_ptr<spdlog::logger> g_fallbackLogger = nullptr;

    // 私有主 logger（避免与用户的 spdlog::default_logger 冲突）
    std::shared_ptr<spdlog::logger> g_internal_logger = nullptr;

    // 用于线程安全初始化
    std::once_flag g_fallbackInitFlag;
    std::once_flag g_loggerInitFlag;
    std::once_flag g_shutdownRegistered;

    // 标识是否使用彩色日志
    std::atomic<bool> g_enableColor{ false };

    // 用于保护 logger 操作的互斥锁
    std::mutex g_logger_mutex;

    // 线程局部变量，用于检测重入
    thread_local bool g_in_callback = false;
}

// ========================================
// CustomSink 定义
// ========================================

class CustomSink : public spdlog::sinks::base_sink<std::mutex>
{
protected:
    void sink_it_(const spdlog::details::log_msg& msg) override {
        // 检测重入：如果已经在回调中，直接使用 fallback 避免死锁
        if (g_in_callback) {
            // 重入场景：直接输出到 stderr，避免死锁
            std::cerr << "[CCKIT Recursive] " << msg.payload.data() << std::endl;
            return;
        }

        // ✅ 修复：在持有锁的情况下复制回调指针，避免 Use-After-Free
        cckit_log_callback_t callback_copy = nullptr;
        void* context_copy = nullptr;

        {
            // 使用原子操作读取回调指针（避免锁）
            callback_copy = g_logCallback;
            context_copy = g_logContext;
        }

        if (callback_copy) {
            // 转换日志级别
            cckit_log_level_t level;
            switch (msg.level) {
            case spdlog::level::trace:    level = CCKIT_LOG_TRACE; break;
            case spdlog::level::debug:    level = CCKIT_LOG_DEBUG; break;
            case spdlog::level::info:     level = CCKIT_LOG_INFO; break;
            case spdlog::level::warn:     level = CCKIT_LOG_WARN; break;
            case spdlog::level::err:      level = CCKIT_LOG_ERROR; break;
            case spdlog::level::critical: level = CCKIT_LOG_CRITICAL; break;
            case spdlog::level::off:      level = CCKIT_LOG_OFF; break;
            default:                      level = CCKIT_LOG_INFO; break;
            }

            // 格式化日志（在持有锁的情况下）
            spdlog::memory_buf_t formatted;
            formatter_->format(msg, formatted);

            // 构造源码位置信息
            cckit_log_source_loc_t loc;
            loc.filename = msg.source.filename ? msg.source.filename : "";
            loc.line = msg.source.line;
            loc.function = msg.source.funcname ? msg.source.funcname : "";

            // 复制数据到缓冲区（在持有锁的情况下）
            std::string formatted_str(formatted.data(), formatted.size());
            cckit_log_source_loc_t loc_copy = loc;

            // ✅ 修复：在释放锁之前标记进入回调
            g_in_callback = true;

            // ✅ 修复：释放锁后调用回调（避免死锁）
            // 注意：这里需要临时释放锁，但 base_sink 的设计不允许这样做
            // 所以我们使用重入检测来避免死锁
            callback_copy(level, &loc_copy, formatted_str.c_str(), formatted_str.size(), context_copy);

            // 标记退出回调
            g_in_callback = false;
        }
        else if (!g_fallbackDisabled) {
            // 延迟初始化 fallback logger
            ensureFallbackLoggerInitialized();
            if (g_fallbackLogger) {
                g_fallbackLogger->log(msg.level,
                    spdlog::string_view_t{ msg.payload.data(), msg.payload.size() });
            }
            else {
                // 极端 fallback：直接输出到 stderr
                std::cerr << std::string_view(msg.payload.data(), msg.payload.size()) << '\n';
            }
        }
        // else: 完全静默
    }

    void flush_() override {}

private:
    // 延迟初始化 fallback logger
    static void ensureFallbackLoggerInitialized() {
        std::call_once(g_fallbackInitFlag, [] {
            // 再次检查，避免竞争条件
            if (g_logCallback || g_fallbackLogger) {
                return;
            }

            try {
                // 创建一个带颜色的 stderr logger
                auto logger = spdlog::stderr_color_mt("cckit_fallback");

#if defined(_DEBUG_MODE) || defined(_DEBUG) || !defined(NDEBUG)
                logger->set_level(spdlog::level::trace);
                logger->set_pattern("[%H:%M:%S] [%^%l%$] [%s:%#] %v");  // 时间 + 颜色 + 文件名:行号 + 消息
#else
                logger->set_level(spdlog::level::info);
                logger->set_pattern("[%H:%M:%S] [%^%l%$] %v");  // 时间 + 颜色 + 消息
#endif

                g_fallbackLogger = std::move(logger);

                // 显示一次警告（提示用户应调用 setLogCallback）
                if (!g_warningShown.exchange(true)) {
                    if (g_logCallback == nullptr) {
                        std::string warning =
                            "[CCKIT Warning] No log callback set.\n"
                            "Using internal colored console logger.\n"
                            "Call cckit_log_set_callback() to integrate with your logging system (e.g., GUI).\n"
                            "[CCKIT Note] Logging inside callbacks is detected and will be output to stderr to avoid deadlock.\n";
                        std::cerr << warning;
                    }
                }
            }
            catch (const spdlog::spdlog_ex& ex) {
                std::cerr << "[CCKIT] Failed to create fallback logger: " << ex.what() << "\n";
            }
        });
    }
};

// ========================================
// 私有 Logger 获取函数
// ========================================

static std::shared_ptr<spdlog::logger> get_internal_logger() {
    std::call_once(g_loggerInitFlag, [] {
        // 创建主 logger
        std::shared_ptr<spdlog::logger> logger = nullptr;

        if (g_enableColor) {
            // 创建一个带颜色的 stderr logger
            logger = spdlog::stderr_color_mt("cckit_private");
        }
        else {
            auto customSink = std::make_shared<CustomSink>();
            logger = std::make_shared<spdlog::logger>("cckit_private", customSink);
        }

#if defined(_DEBUG_MODE) || defined(_DEBUG) || !defined(NDEBUG)
        logger->set_level(spdlog::level::trace);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] [%s:%#] %v");
#else
        logger->set_level(spdlog::level::info);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
#endif

        logger->flush_on(spdlog::level::info);

        g_internal_logger = logger;
    });
    return g_internal_logger;
}

// 日志级别转换辅助函数
static spdlog::level::level_enum cckit_level_to_spdlog(cckit_log_level_t level) {
    switch (level) {
    case CCKIT_LOG_TRACE:    return spdlog::level::trace;
    case CCKIT_LOG_DEBUG:    return spdlog::level::debug;
    case CCKIT_LOG_INFO:     return spdlog::level::info;
    case CCKIT_LOG_WARN:     return spdlog::level::warn;
    case CCKIT_LOG_ERROR:    return spdlog::level::err;
    case CCKIT_LOG_CRITICAL: return spdlog::level::critical;
    case CCKIT_LOG_OFF:      return spdlog::level::off;
    default:                 return spdlog::level::info;
    }
}

static cckit_log_level_t spdlog_level_to_cckit(spdlog::level::level_enum level) {
    switch (level) {
    case spdlog::level::trace:    return CCKIT_LOG_TRACE;
    case spdlog::level::debug:    return CCKIT_LOG_DEBUG;
    case spdlog::level::info:     return CCKIT_LOG_INFO;
    case spdlog::level::warn:     return CCKIT_LOG_WARN;
    case spdlog::level::err:      return CCKIT_LOG_ERROR;
    case spdlog::level::critical: return CCKIT_LOG_CRITICAL;
    case spdlog::level::off:      return CCKIT_LOG_OFF;
    default:                      return CCKIT_LOG_INFO;
    }
}

// ========================================
// C++ 接口实现
// ========================================

namespace cckit::logging
{

    void setLogCallback(LogCallback callback, void* context, bool enableColor)
    {
        if (!callback) {
            detail::CallbackManager::instance().clearCallback();
            cckit_log_set_callback(nullptr, context, enableColor);
            return;
        }

        // 使用线程安全的回调管理器
        detail::CallbackManager::instance().setCallback(std::move(callback), context);

        // 创建 C 回调包装器（捕获回调管理器的引用）
        auto cCallback = [](cckit_log_level_t level, const cckit_log_source_loc_t* loc,
                            const char* msg, size_t len, void* ctx) {
            // ✅ 修复：获取 shared_ptr，增加引用计数，确保对象生命周期
            auto [callbackSharedPtr, contextPtr] = detail::CallbackManager::instance().getCallback();
            if (callbackSharedPtr && *callbackSharedPtr) {
                SourceLoc sourceLoc(loc ? loc->filename : "", loc ? loc->line : 0, loc ? loc->function : "");
                std::string message(msg, len);
                // ✅ 安全调用：shared_ptr 保证对象有效
                (*callbackSharedPtr)(static_cast<LogLevel>(level), sourceLoc, message, ctx);
            }
        };

        cckit_log_set_callback(cCallback, context, enableColor);
    }

    LogCallback getLogCallback() {
        //auto cCallback = cckit_log_get_callback();
        // 注意：这里无法从 C 回调恢复 C++ 回调，所以总是返回空指针
        return nullptr;
    }

    std::shared_ptr<spdlog::logger> getLogger() {
        void* rawPtr = cckit_log_get_logger();
        if (rawPtr) {
            // 直接从全局 registry 获取私有 logger
            auto logger = spdlog::get("cckit_private");
            return logger;
        }
        return nullptr;
    }

} // namespace cckit

// ========================================
// C API 接口实现
// ========================================

extern "C"
{

    // ========================================
    // 日志级别控制
    // ========================================

    cckit_log_level_t cckit_log_get_level(void)
    {
        auto logger = get_internal_logger();
        if (logger) {
            return spdlog_level_to_cckit(logger->level());
        }
        return CCKIT_LOG_INFO;
    }

    void cckit_log_set_level(cckit_log_level_t level)
    {
        auto logger = get_internal_logger();
        if (logger) {
            logger->set_level(cckit_level_to_spdlog(level));
        }
    }

    // ========================================
    // 日志回调机制
    // ========================================

    void cckit_log_set_callback(cckit_log_callback_t callback, void* context, bool enableColor)
    {
        g_logCallback = callback;
        g_logContext = context;
        g_enableColor = enableColor;

        // 初始化私有 logger（懒加载）
        get_internal_logger();
    }

    cckit_log_callback_t cckit_log_get_callback(void)
    {
        return g_logCallback;
    }

    void* cckit_log_get_callback_context(void)
    {
        return g_logContext;
    }

    // ========================================
    // Fallback 机制
    // ========================================

    void cckit_log_disable_fallback(void)
    {
        g_fallbackDisabled = true;
    }

    void cckit_log_enable_fallback(void)
    {
        g_fallbackDisabled = false;
    }

    // ========================================
    // 基础日志函数
    // ========================================

    void cckit_log_trace(const char* msg) {
        auto logger = get_internal_logger();
        if (logger) logger->trace(msg);
    }

    void cckit_log_debug(const char* msg) {
        auto logger = get_internal_logger();
        if (logger) logger->debug(msg);
    }

    void cckit_log_info(const char* msg) {
        auto logger = get_internal_logger();
        if (logger) logger->info(msg);
    }

    void cckit_log_warn(const char* msg) {
        auto logger = get_internal_logger();
        if (logger) logger->warn(msg);
    }

    void cckit_log_error(const char* msg) {
        auto logger = get_internal_logger();
        if (logger) logger->error(msg);
    }

    void cckit_log_critical(const char* msg) {
        auto logger = get_internal_logger();
        if (logger) logger->critical(msg);
    }

    // ========================================
    // 带源码位置的日志函数
    // ========================================

    void cckit_log_log(cckit_log_level_t level, const cckit_log_source_loc_t* loc, const char* msg)
    {
        auto logger = get_internal_logger();
        if (!logger) {
            return;
        }

        // 转换日志级别
        spdlog::level::level_enum spdlog_level = cckit_level_to_spdlog(level);

        // 构造源码位置
        spdlog::source_loc source_loc;
        if (loc) {
            source_loc.filename = loc->filename ? loc->filename : "";
            source_loc.funcname = loc->function ? loc->function : "";
            source_loc.line = loc->line;
        } else {
            source_loc.filename = "";
            source_loc.funcname = "";
            source_loc.line = 0;
        }

        // 构造日志消息
        spdlog::details::log_msg log_msg(source_loc, "cckit_private", spdlog_level, msg);

        // 直接输出到私有 logger
        //logger->log(log_msg);

        //log_msg 是内部类型 - details::log_msg 是 spdlog 内部使用的类型
        //Sink 接受 log_msg - sink 的 log() 方法接受 log_msg 参数
        //绕过 logger 层 - 直接调用 sink 可以传递完整的 log_msg 对象
        logger->sinks()[0]->log(log_msg);
    }

    void cckit_log_logf(cckit_log_level_t level, const cckit_log_source_loc_t* loc, const char* fmt, ...)
    {
        if (fmt == nullptr) {
            return;
        }

        // 格式化消息
        char buffer[4096];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);

        // 调用非格式化版本
        cckit_log_log(level, loc, buffer);
    }

    // ========================================
    // 日志生命周期管理
    // ========================================

    void cckit_log_initialize(void)
    {
        // 确保 logger 已初始化
        get_internal_logger();
    }

    void cckit_log_shutdown(void)
    {
        // ✅ 修复：先清除回调，避免在持有锁的情况下调用回调
        g_logCallback = nullptr;
        g_logContext = nullptr;

        // ✅ 修复：使用 try_lock 避免死锁
        std::unique_lock<std::mutex> lock(g_logger_mutex, std::try_to_lock);
        if (!lock.owns_lock()) {
            // 如果无法获取锁，说明还有线程在使用 logger
            // 等待一段时间后重试
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            lock.lock();
        }

        // 只清理自己的资源
        if (g_internal_logger) {
            g_internal_logger->flush();
            g_internal_logger.reset();
        }

        if (g_fallbackLogger) {
            g_fallbackLogger.reset();
        }

        // 重置全局状态
        g_enableColor = false;
    }

    void cckit_log_flush(void)
    {
        auto logger = get_internal_logger();
        if (logger) {
            logger->flush();
        }

        if (g_fallbackLogger) {
            g_fallbackLogger->flush();
        }
    }

    // ========================================
    // Logger 获取接口
    // ========================================

    void* cckit_log_get_logger(void)
    {
        auto logger = get_internal_logger();
        return logger.get();
    }

} // extern "C"
