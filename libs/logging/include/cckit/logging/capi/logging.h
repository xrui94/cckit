// log.h - 日志 C 接口
#pragma once

#include "../export.h"

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

    // 日志级别枚举
    typedef enum {
        CCKIT_LOG_TRACE = 0,
        CCKIT_LOG_DEBUG = 1,
        CCKIT_LOG_INFO = 2,
        CCKIT_LOG_WARN = 3,
        CCKIT_LOG_ERROR = 4,
        CCKIT_LOG_CRITICAL = 5,
        CCKIT_LOG_OFF = 6
    } cckit_log_level_t;

    // 源码位置信息
    typedef struct {
        const char* filename;  // 文件名
        int line;             // 行号
        const char* function; // 函数名
    } cckit_log_source_loc_t;

    // 日志回调函数类型
    // @param level 日志级别
    // @param loc 源码位置信息
    // @param msg 日志消息
    // @param len 消息长度
    // @param context 用户上下文指针
    typedef void(C_CALL* cckit_log_callback_t)(
        cckit_log_level_t level,
        const cckit_log_source_loc_t* loc,
        const char* msg,
        size_t len,
        void* context
    );

    // ========================================
    // 日志级别控制
    // ========================================

    /**
     * @brief 设置日志级别
     * @param level 日志级别（低于该级别的日志将被过滤）
     */
    CCKIT_API void cckit_log_set_level(cckit_log_level_t level);

    /**
     * @brief 获取当前日志级别
     * @return 当前日志级别
     */
    CCKIT_API cckit_log_level_t cckit_log_get_level(void);

    // ========================================
    // 日志回调机制
    // ========================================

    /**
     * @brief 设置日志回调函数
     * @param callback 回调函数指针，传入 nullptr 可禁用回调
     * @param context 用户上下文指针，会在回调时原样传回
     * @param enableColor 是否启用带颜色的日志格式（仅用于 fallback 输出）
     *
     * @note 该函数可安全被多次调用，后续调用会更新回调和上下文。
     * @note 回调函数应在短时间内返回，避免阻塞日志线程。
     *
     * @warning ⚠️ 重要：在日志回调函数内部再次调用 cckit 日志函数会导致该条日志被重定向到标准错误输出 (stderr)，以避免死锁。
     *          这是由于 spdlog 的 base_sink 使用了不可重入的 std::mutex，在持有锁的情况下递归调用会导致死锁。
     *          为了保证系统稳定性，cckit 检测到重入时会自动降级到 stderr 输出，格式为 "[CCKIT Recursive] 消息内容"。
     *
     * @code
     * // ❌ 不推荐：在回调中记录日志
     * void my_callback(...) {
     *     cckit_log_info("这条日志会被重定向到 stderr");  // 重入！
     * }
     *
     * // ✅ 推荐：回调中只做简单的日志转发
     * void my_callback(...) {
     *     // 将日志发送到 GUI、文件或其他系统
     *     send_to_gui(msg);
     * }
     * @endcode
     */
    CCKIT_API void cckit_log_set_callback(cckit_log_callback_t callback, void* context, bool enableColor);

    /**
     * @brief 获取当前日志回调函数
     * @return 当前回调函数指针
     */
    CCKIT_API cckit_log_callback_t cckit_log_get_callback(void);

    /**
     * @brief 获取当前用户上下文
     * @return 当前用户上下文指针
     */
    CCKIT_API void* cckit_log_get_callback_context(void);

    // ========================================
    // Fallback 机制
    // ========================================

    /**
     * @brief 禁用 fallback 日志
     *
     * 当回调函数为空或无法调用时，默认会输出到控制台（stderr）。
     * 调用此函数可禁用此行为，使日志静默。
     */
    CCKIT_API void cckit_log_disable_fallback(void);

    /**
     * @brief 启用 fallback 日志
     */
    CCKIT_API void cckit_log_enable_fallback(void);

    // ========================================
    // 基础日志函数
    // ========================================

    CCKIT_API void cckit_log_trace(const char* msg);
    CCKIT_API void cckit_log_debug(const char* msg);
    CCKIT_API void cckit_log_info(const char* msg);
    CCKIT_API void cckit_log_warn(const char* msg);
    CCKIT_API void cckit_log_error(const char* msg);
    CCKIT_API void cckit_log_critical(const char* msg);

    // ========================================
    // 带源码位置的日志函数
    // ========================================

    /**
     * @brief 带源码位置的日志输出
     * @param level 日志级别
     * @param loc 源码位置信息
     * @param msg 日志消息
     */
    CCKIT_API void cckit_log_log(cckit_log_level_t level, const cckit_log_source_loc_t* loc, const char* msg);

    /**
     * @brief 带源码位置的格式化日志输出
     * @param level 日志级别
     * @param loc 源码位置信息
     * @param fmt 格式化字符串
     * @param ... 可变参数
     */
    CCKIT_API void cckit_log_logf(cckit_log_level_t level, const cckit_log_source_loc_t* loc, const char* fmt, ...);

    // ========================================
    // 日志生命周期管理
    // ========================================

    /**
     * @brief 初始化日志系统
     *
     * 创建默认的 spdlog logger 并注册进程退出时的清理函数。
     * 通常在程序启动时调用一次即可，后续调用无副作用。
     */
    CCKIT_API void cckit_log_initialize(void);

    /**
     * @brief 关闭并清理日志系统资源
     *
     * 手动销毁所有 spdlog logger、sink 和内部资源。
     * 建议在以下场景调用：
     * - 程序正常退出前，确保日志完整刷新
     * - 反复加载/卸载依赖本静态库的 DLL 时
     * - 需要重新初始化日志系统前
     *
     * @note 可安全重复调用，多次调用无副作用。
     */
    CCKIT_API void cckit_log_shutdown(void);

    /**
     * @brief 刷新日志缓冲区
     *
     * 强制刷新所有日志输出，确保日志被立即写入。
     */
    CCKIT_API void cckit_log_flush(void);

    /**
     * @brief 获取默认 logger 指针
     *
     * 返回 spdlog 默认 logger 的原始指针，用于高级用户直接操作 spdlog。
     * 注意：返回的指针生命周期由 spdlog 管理，不要手动释放。
     *
     * @return spdlog::logger 的原始指针，如果未初始化则返回 nullptr
     */
    CCKIT_API void* cckit_log_get_logger(void);

#ifdef __cplusplus
}
#endif

// ========================================
// 简化日志宏
// ========================================

/**
 * @brief 简化日志宏
 *
 * 提供不带 CCKIT_ 前缀的简化宏，方便用户使用。
 * 使用 #ifndef 保护，避免与其他库的宏冲突。
 *
 * @note 如果需要完全禁用这些简化宏，可以在包含 log.h 之前定义 CCKIT_DISABLE_SIMPLE_MACROS
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
