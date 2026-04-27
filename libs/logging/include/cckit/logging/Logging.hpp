// log.hpp - 日志 C++ 包装层
#pragma once

#include "capi/logging.h"

// ✅ 直接使用 fmt 库，提供完整的格式化功能
// 注意：spdlog 内置了 fmt，但直接使用 fmt 可以更明确依赖关系
// 未来 C++23 将引入 std::format，届时可以平滑迁移
#include <fmt/format.h>

#include <string>
#include <functional>
#include <memory>
#include <mutex>

// 前向声明，避免暴露 spdlog 依赖
namespace spdlog { class logger; }

namespace cckit::logging
{

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
            : filename(file), line(ln), function(func) {
        }

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
    // 回调管理器 - 线程安全且无内存泄漏
    // ========================================

    namespace detail {
        // 线程安全的回调包装器管理
        class CallbackManager {
        public:
            static CallbackManager& instance() {
                static CallbackManager instance;
                return instance;
            }

            void setCallback(LogCallback callback, void* context) {
                std::lock_guard<std::mutex> lock(mutex_);
                // 使用 shared_ptr 管理回调，确保线程安全
                callback_ = std::make_shared<LogCallback>(std::move(callback));
                context_ = context;
            }

            void clearCallback() {
                std::lock_guard<std::mutex> lock(mutex_);
                callback_.reset();
                context_ = nullptr;
            }

            // ✅ 修复：返回 shared_ptr，避免 Use-After-Free
            std::pair<std::shared_ptr<LogCallback>, void*> getCallback() const {
                std::lock_guard<std::mutex> lock(mutex_);
                // 增加 shared_ptr 引用计数，确保对象生命周期
                return { callback_, context_ };
            }

            void* getContext() const {
                std::lock_guard<std::mutex> lock(mutex_);
                return context_;
            }

        private:
            CallbackManager() = default;
            ~CallbackManager() = default;
            CallbackManager(const CallbackManager&) = delete;
            CallbackManager& operator=(const CallbackManager&) = delete;

            mutable std::mutex mutex_;
            // ✅ 使用 shared_ptr 替代 unique_ptr
            std::shared_ptr<LogCallback> callback_;
            void* context_ = nullptr;
        };
    }

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
     *
     * @note 此函数是线程安全的，可以安全地从多个线程调用
     * @note 使用 std::shared_ptr 管理回调对象，避免 Use-After-Free
     *
     * @warning ⚠️ 重要：在日志回调函数内部再次调用日志宏会导致该条日志被重定向到标准错误输出 (stderr)，以避免死锁。
     *          这是由于 spdlog 的 base_sink 使用了不可重入的 std::mutex，在持有锁的情况下递归调用会导致死锁。
     *          为了保证系统稳定性，cckit 检测到重入时会自动降级到 stderr 输出，格式为 "[CCKIT Recursive] 消息内容"。
     *
     * @code
     * // ❌ 不推荐：在回调中记录日志
     * cckit::logging::setLogCallback([](...) {
     *     cckit::logging::logInfo("这条日志会被重定向到 stderr");  // 重入！
     * });
     *
     * // ✅ 推荐：回调中只做简单的日志转发
     * cckit::logging::setLogCallback([](...) {
     *     // 将日志发送到 GUI、文件或其他系统
     *     send_to_gui(msg);
     * });
     * @endcode
     */
    void setLogCallback(LogCallback callback, void* context = nullptr, bool enableColor = false);

    // 注意：由于内部原因，目前，该函数总是返回空指针
    LogCallback getLogCallback();

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

    inline void logTrace(const std::string& msg) {
        cckit_log_source_loc_t cLoc = SourceLoc().toC();
        cckit_log_log(static_cast<cckit_log_level_t>(LogLevel::Trace), &cLoc, msg.c_str());
    }

    inline void logDebug(const std::string& msg) {
        cckit_log_source_loc_t cLoc = SourceLoc().toC();
        cckit_log_log(static_cast<cckit_log_level_t>(LogLevel::Debug), &cLoc, msg.c_str());
    }

    inline void logInfo(const std::string& msg)  {
        cckit_log_source_loc_t cLoc = SourceLoc().toC();
        cckit_log_log(static_cast<cckit_log_level_t>(LogLevel::Info), &cLoc, msg.c_str());
    }

    inline void logWarn(const std::string& msg)  {
        cckit_log_source_loc_t cLoc = SourceLoc().toC();
        cckit_log_log(static_cast<cckit_log_level_t>(LogLevel::Warn), &cLoc, msg.c_str());
    }

    inline void logError(const std::string& msg) {
        cckit_log_source_loc_t cLoc = SourceLoc().toC();
        cckit_log_log(static_cast<cckit_log_level_t>(LogLevel::Error), &cLoc, msg.c_str());
    }

    inline void logCritical(const std::string& msg) {
        cckit_log_source_loc_t cLoc = SourceLoc().toC();
        cckit_log_log(static_cast<cckit_log_level_t>(LogLevel::Critical), &cLoc, msg.c_str());
    }

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

        // ✅ 使用 fmt 库进行格式化（spdlog 内置，C++17 兼容）
        template<typename... Args>
        inline void logMessage(LogLevel level, const SourceLoc& loc, const char* fmt, const Args&... args) {
            try {
                // 使用 fmt 进行格式化，支持完整的格式化功能
                auto formatted = fmt::format(fmt, args...);
                log(level, loc, formatted);
            }
            catch (const fmt::format_error& e) {
                // 格式化失败时，记录原始格式字符串
                log(level, loc, fmt);
            }
            catch (...) {
                // 其他异常，记录原始格式字符串
                log(level, loc, fmt);
            }
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
    // Logger 获取接口
    // ========================================

    /**
     * @brief 获取 cckit 私有 spdlog logger
     *
     * 返回 cckit 内部使用的私有 spdlog logger 的共享指针。
     * 注意：这是一个私有实例，不会与用户的 spdlog::default_logger 冲突。
     *
     * @return cckit 私有 logger 的共享指针，如果未初始化则返回空指针
     *
     * @note 此函数返回的 logger 是 cckit 的私有实例，与用户代码中的 spdlog 完全隔离
     * @note 此函数在 log.cpp 中实现，以避免在头文件中暴露 spdlog 依赖
     */
    std::shared_ptr<spdlog::logger> getLogger();

} // namespace cckit::logging


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
// 注意：必须定义在命名空间外部
// ========================================

#define CCKIT_LOG_TRACE(...)  do { if (CCKIT_LOG_LEVEL_TRACE >= CCKIT_MIN_LOG_LEVEL)     cckit::logging::logTrace(cckit::logging::SourceLoc(), __VA_ARGS__); } while(0)
#define CCKIT_LOG_DEBUG(...)  do { if (CCKIT_LOG_LEVEL_DEBUG >= CCKIT_MIN_LOG_LEVEL)     cckit::logging::logDebug(cckit::logging::SourceLoc(), __VA_ARGS__); } while(0)
#define CCKIT_LOG_INFO(...)   do { if (CCKIT_LOG_LEVEL_INFO  >= CCKIT_MIN_LOG_LEVEL)      cckit::logging::logInfo(cckit::logging::SourceLoc(), __VA_ARGS__); } while(0)
#define CCKIT_LOG_WARN(...)   do { if (CCKIT_LOG_LEVEL_WARN  >= CCKIT_MIN_LOG_LEVEL)      cckit::logging::logWarn(cckit::logging::SourceLoc(), __VA_ARGS__); } while(0)
#define CCKIT_LOG_ERROR(...)  do { if (CCKIT_LOG_LEVEL_ERROR >= CCKIT_MIN_LOG_LEVEL)     cckit::logging::logError(cckit::logging::SourceLoc(), __VA_ARGS__); } while(0)
#define CCKIT_LOG_CRITICAL(...) do { if (CCKIT_LOG_LEVEL_CRITICAL >= CCKIT_MIN_LOG_LEVEL) cckit::logging::logCritical(cckit::logging::SourceLoc(), __VA_ARGS__); } while(0)

// ========================================
// 简化日志宏
// ========================================

/**
 * @brief 简化日志宏
 *
 * 提供不带 CCKIT_ 前缀的简化宏，方便用户使用。
 * 使用 #ifndef 保护，避免与其他库的宏冲突。
 *
 * @note 如果需要完全禁用这些简化宏，可以在包含 log.hpp 之前定义 CCKIT_DISABLE_SIMPLE_MACROS
 *
 * @code
 * // 使用简化宏（推荐）
 * LOG_INFO("Hello, World!");
 *
 * // 使用完整宏（避免命名冲突）
 * CCKIT_LOG_INFO("Hello, World!");
 * @endcode
 */

#ifndef CCKIT_DISABLE_SIMPLE_MACROS

    // 只在未定义时提供简化宏
    #ifndef LOG_TRACE
        #define LOG_TRACE(...)    CCKIT_LOG_TRACE(__VA_ARGS__)
    #endif

    #ifndef LOG_DEBUG
        #define LOG_DEBUG(...)    CCKIT_LOG_DEBUG(__VA_ARGS__)
    #endif

    #ifndef LOG_INFO
        #define LOG_INFO(...)     CCKIT_LOG_INFO(__VA_ARGS__)
    #endif

    #ifndef LOG_WARN
        #define LOG_WARN(...)     CCKIT_LOG_WARN(__VA_ARGS__)
    #endif

    #ifndef LOG_ERROR
        #define LOG_ERROR(...)    CCKIT_LOG_ERROR(__VA_ARGS__)
    #endif

    #ifndef LOG_CRITICAL
        #define LOG_CRITICAL(...) CCKIT_LOG_CRITICAL(__VA_ARGS__)
    #endif

#endif // CCKIT_DISABLE_SIMPLE_MACROS
