// log.hpp - 日志 C++ 包装层
#pragma once

#include "log.h"
#include <string>
#include <functional>
#include <memory>

namespace cckit {

// ========================================
// 日志级别枚举
// ========================================

enum class LogLevel
{
    Trace = CCKIT_LOG_TRACE,
    Debug = CCKIT_LOG_DEBUG,
    Info = CCKIT_LOG_INFO,
    Warn = CCKIT_LOG_WARN,
    Error = CCKIT_LOG_ERROR,
    Critical = CCKIT_LOG_CRITICAL,
    Off = CCKIT_LOG_OFF
};

// ========================================
// 源码位置信息
// ========================================

struct SourceLoc {
    const char* filename;
    int line;
    const char* function;

    SourceLoc(const char* file = __builtin_FILE(), int ln = __builtin_LINE(), const char* func = __builtin_FUNCTION())
        : filename(file), line(ln), function(func) {}

    // 转换为 C 接口类型
    cckit_log_source_loc_t toC() const {
        cckit_log_source_loc_t loc;
        loc.filename = filename;
        loc.line = line;
        loc.function = function;
        return loc;
    }
};

// ========================================
// 日志回调函数类型
// ========================================

using LogCallback = std::function<void(LogLevel level, const SourceLoc& loc, const std::string& msg, void* context)>;

// ========================================
// 日志级别控制
// ========================================

inline void setLogLevel(LogLevel level) {
    cckit_log_set_level(static_cast<cckit_log_level_t>(level));
}

inline LogLevel getLogLevel() {
    return static_cast<LogLevel>(cckit_log_get_level());
}

// ========================================
// 日志回调机制
// ========================================

/**
 * @brief 设置日志回调函数
 * @param callback 回调函数
 * @param context 用户上下文指针
 * @param enableColor 是否启用颜色
 */
inline void setLogCallback(LogCallback callback, void* context = nullptr, bool enableColor = false) {
    if (!callback) {
        cckit_log_set_callback(nullptr, context, enableColor);
        return;
    }

    // 创建 C 回调包装器
    static auto* callbackWrapper = new LogCallback(callback);
    static auto* contextWrapper = context;

    auto cCallback = [](cckit_log_level_t level, const cckit_log_source_loc_t* loc, const char* msg, size_t len, void* ctx) {
        if (*callbackWrapper) {
            SourceLoc sourceLoc(loc ? loc->filename : "", loc ? loc->line : 0, loc ? loc->function : "");
            std::string message(msg, len);
            (*callbackWrapper)(static_cast<LogLevel>(level), sourceLoc, message, ctx);
        }
    };

    cckit_log_set_callback(cCallback, context, enableColor);
}

inline LogCallback getLogCallback() {
    auto cCallback = cckit_log_get_callback();
    // 注意：这里无法从 C 回调恢复 C++ 回调，所以返回空
    return nullptr;
}

inline void* getLogCallbackContext() {
    return cckit_log_get_callback_context();
}

// ========================================
// Fallback 机制
// ========================================

inline void disableFallbackLogging() {
    cckit_log_disable_fallback();
}

inline void enableFallbackLogging() {
    cckit_log_enable_fallback();
}

// ========================================
// 基础日志函数
// ========================================

inline void logTrace(const std::string& msg) { cckit_log_trace(msg.c_str()); }
inline void logDebug(const std::string& msg) { cckit_log_debug(msg.c_str()); }
inline void logInfo(const std::string& msg)  { cckit_log_info(msg.c_str()); }
inline void logWarn(const std::string& msg)  { cckit_log_warn(msg.c_str()); }
inline void logError(const std::string& msg) { cckit_log_error(msg.c_str()); }
inline void logCritical(const std::string& msg) { cckit_log_critical(msg.c_str()); }

// ========================================
// 带源码位置的日志函数
// ========================================

inline void log(LogLevel level, const SourceLoc& loc, const std::string& msg) {
    cckit_log_source_loc_t cLoc = loc.toC();
    cckit_log_log(static_cast<cckit_log_level_t>(level), &cLoc, msg.c_str());
}

// ========================================
// 模板化日志函数（支持格式化）
// ========================================

namespace detail {
    inline void logMessage(LogLevel level, const SourceLoc& loc, const char* fmt) {
        log(level, loc, fmt);
    }

    template<typename... Args>
    inline void logMessage(LogLevel level, const SourceLoc& loc, const char* fmt, const Args&... args) {
        // 格式化消息
        int size = std::snprintf(nullptr, 0, fmt, args...) + 1;
        if (size <= 0) {
            log(level, loc, fmt);
            return;
        }

        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(buf.get(), size, fmt, args...);
        log(level, loc, std::string(buf.get(), size - 1));
    }
}

// 带源码位置的模板化日志函数
template<typename... Args>
inline void logTrace(const SourceLoc& loc, const char* fmt, const Args&... args) {
    detail::logMessage(LogLevel::Trace, loc, fmt, args...);
}

template<typename... Args>
inline void logDebug(const SourceLoc& loc, const char* fmt, const Args&... args) {
    detail::logMessage(LogLevel::Debug, loc, fmt, args...);
}

template<typename... Args>
inline void logInfo(const SourceLoc& loc, const char* fmt, const Args&... args) {
    detail::logMessage(LogLevel::Info, loc, fmt, args...);
}

template<typename... Args>
inline void logWarn(const SourceLoc& loc, const char* fmt, const Args&... args) {
    detail::logMessage(LogLevel::Warn, loc, fmt, args...);
}

template<typename... Args>
inline void logError(const SourceLoc& loc, const char* fmt, const Args&... args) {
    detail::logMessage(LogLevel::Error, loc, fmt, args...);
}

template<typename... Args>
inline void logCritical(const SourceLoc& loc, const char* fmt, const Args&... args) {
    detail::logMessage(LogLevel::Critical, loc, fmt, args...);
}

// ========================================
// 日志生命周期管理
// ========================================

inline void initializeLogging() {
    cckit_log_initialize();
}

inline void shutdownLogging() {
    cckit_log_shutdown();
}

inline void flushLogging() {
    cckit_log_flush();
}

// ========================================
// 编译期日志级别控制
// ========================================

// 整数级别，用于宏比较
#define CCKIT_LOG_LEVEL_TRACE     0
#define CCKIT_LOG_LEVEL_DEBUG     1
#define CCKIT_LOG_LEVEL_INFO      2
#define CCKIT_LOG_LEVEL_WARN      3
#define CCKIT_LOG_LEVEL_ERROR     4
#define CCKIT_LOG_LEVEL_CRITICAL  5

// 用户可通过 -DCCKIT_MIN_LOG_LEVEL=x 控制最低输出级别
// 未定义时，根据模式自动设置
#ifndef CCKIT_MIN_LOG_LEVEL
    #if defined(_DEBUG_MODE) || defined(_DEBUG) || !defined(NDEBUG)
        #define CCKIT_MIN_LOG_LEVEL CCKIT_LOG_LEVEL_TRACE  // Debug: 输出所有级别
    #else
        #define CCKIT_MIN_LOG_LEVEL CCKIT_LOG_LEVEL_INFO   // Release: 只输出 INFO 及以上
    #endif
#endif

// ========================================
// 带级别检查的日志宏（推荐使用）
// ========================================

#define CCKIT_LOG_TRACE(...)  do { if (CCKIT_LOG_LEVEL_TRACE >= CCKIT_MIN_LOG_LEVEL)     cckit::logTrace(cckit::SourceLoc(), __VA_ARGS__); } while(0)
#define CCKIT_LOG_DEBUG(...)  do { if (CCKIT_LOG_LEVEL_DEBUG >= CCKIT_MIN_LOG_LEVEL)     cckit::logDebug(cckit::SourceLoc(), __VA_ARGS__); } while(0)
#define CCKIT_LOG_INFO(...)   do { if (CCKIT_LOG_LEVEL_INFO  >= CCKIT_MIN_LOG_LEVEL)      cckit::logInfo(cckit::SourceLoc(), __VA_ARGS__); } while(0)
#define CCKIT_LOG_WARN(...)   do { if (CCKIT_LOG_LEVEL_WARN  >= CCKIT_MIN_LOG_LEVEL)      cckit::logWarn(cckit::SourceLoc(), __VA_ARGS__); } while(0)
#define CCKIT_LOG_ERROR(...)  do { if (CCKIT_LOG_LEVEL_ERROR >= CCKIT_MIN_LOG_LEVEL)     cckit::logError(cckit::SourceLoc(), __VA_ARGS__); } while(0)
#define CCKIT_LOG_CRITICAL(...) do { if (CCKIT_LOG_LEVEL_CRITICAL >= CCKIT_MIN_LOG_LEVEL) cckit::logCritical(cckit::SourceLoc(), __VA_ARGS__); } while(0)

} // namespace cckit
