// runtime.h - 运行时工具 C 接口
#pragma once

#include "cckit/export.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// 可执行文件路径
// ========================================

/**
 * @brief 获取当前可执行文件的完整路径
 * @return 新分配的路径字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_executable_path(void);

/**
 * @brief 获取当前可执行文件所在的目录
 * @return 新分配的目录路径字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_executable_dir(void);

// ========================================
// 动态库路径
// ========================================

/**
 * @brief 获取当前动态库的完整路径（包括文件名）
 *
 * 此函数确定其自身所在的动态链接库（DLL/so/dylib）的完整文件路径。
 *
 * @return 新分配的路径字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_library_path(void);

/**
 * @brief 获取当前动态库所在的目录路径
 *
 * 此函数调用 cckit_runtime_get_library_path 获取完整路径，然后提取目录部分。
 *
 * @return 新分配的目录路径字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_library_dir(void);

// ========================================
// 进程信息
// ========================================

/**
 * @brief 获取当前进程 ID
 * @return 进程 ID
 */
CCKIT_API int cckit_runtime_get_process_id(void);

/**
 * @brief 获取父进程 ID
 * @return 父进程 ID，如果获取失败返回 -1
 */
CCKIT_API int cckit_runtime_get_parent_process_id(void);

/**
 * @brief 获取当前进程名称
 * @return 新分配的进程名称字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_process_name(void);

/**
 * @brief 获取命令行参数数量
 * @return 命令行参数数量
 */
CCKIT_API int cckit_runtime_get_argc(void);

/**
 * @brief 获取命令行参数数组
 * @return 命令行参数数组指针，不要释放
 */
CCKIT_API char** cckit_runtime_get_argv(void);

/**
 * @brief 获取完整的命令行字符串
 * @return 新分配的命令行字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_command_line(void);

// ========================================
// 环境变量
// ========================================

/**
 * @brief 获取环境变量
 * @param name 环境变量名称
 * @return 新分配的环境变量值字符串，如果不存在返回 NULL，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_env(const char* name);

/**
 * @brief 获取环境变量，如果不存在则返回默认值
 * @param name 环境变量名称
 * @param default_value 默认值
 * @return 新分配的环境变量值字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_env_or_default(const char* name, const char* default_value);

/**
 * @brief 设置环境变量
 * @param name 环境变量名称
 * @param value 环境变量值
 * @return true 如果设置成功
 */
CCKIT_API bool cckit_runtime_set_env(const char* name, const char* value);

/**
 * @brief 删除环境变量
 * @param name 环境变量名称
 * @return true 如果删除成功
 */
CCKIT_API bool cckit_runtime_unset_env(const char* name);

/**
 * @brief 获取 PATH 环境变量中的路径列表
 * @param out_count 输出：路径数量
 * @return 新分配的路径数组，调用者需使用 cckit_runtime_free_string_array 释放
 */
CCKIT_API char** cckit_runtime_get_path_env(size_t* out_count);

// ========================================
// 工作目录
// ========================================

/**
 * @brief 获取当前工作目录
 * @return 新分配的当前目录路径字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_current_working_directory(void);

/**
 * @brief 设置当前工作目录
 * @param path 新的工作目录路径
 * @return true 如果设置成功
 */
CCKIT_API bool cckit_runtime_set_current_working_directory(const char* path);

/**
 * @brief 获取临时目录
 * @return 新分配的临时目录路径字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_temp_directory(void);

/**
 * @brief 创建临时目录
 * @param prefix 目录前缀（可以为 NULL，默认为 "cckit_"）
 * @return 新分配的临时目录路径字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_create_temp_directory(const char* prefix);

/**
 * @brief 创建临时文件
 * @param prefix 文件前缀（可以为 NULL，默认为 "cckit_"）
 * @return 新分配的临时文件路径字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_create_temp_file(const char* prefix);

// ========================================
// 标准路径
// ========================================

/**
 * @brief 获取用户主目录
 * @return 新分配的主目录路径字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_home_directory(void);

/**
 * @brief 获取用户配置目录
 * @return 新分配的配置目录路径字符串，调用者需使用 cckit_runtime_free_string 释放
 *
 * 平台差异：
 * - Windows: C:\Users\username\AppData\Roaming
 * - Linux: /home/username/.config
 * - macOS: /Users/username/Library/Application Support
 */
CCKIT_API char* cckit_runtime_get_user_config_directory(void);

/**
 * @brief 获取用户数据目录
 * @return 新分配的数据目录路径字符串，调用者需使用 cckit_runtime_free_string 释放
 *
 * 平台差异：
 * - Windows: C:\Users\username\AppData\Local
 * - Linux: /home/username/.local/share
 * - macOS: /Users/username/Library/Application Support
 */
CCKIT_API char* cckit_runtime_get_user_data_directory(void);

/**
 * @brief 获取用户缓存目录
 * @return 新分配的缓存目录路径字符串，调用者需使用 cckit_runtime_free_string 释放
 *
 * 平台差异：
 * - Windows: C:\Users\username\AppData\Local\Temp
 * - Linux: /home/username/.cache
 * - macOS: /Users/username/Library/Caches
 */
CCKIT_API char* cckit_runtime_get_user_cache_directory(void);

/**
 * @brief 获取应用程序配置目录
 * @param app_name 应用程序名称
 * @return 新分配的配置目录路径字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_app_config_directory(const char* app_name);

/**
 * @brief 获取应用程序数据目录
 * @param app_name 应用程序名称
 * @return 新分配的数据目录路径字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_app_data_directory(const char* app_name);

/**
 * @brief 获取应用程序缓存目录
 * @param app_name 应用程序名称
 * @return 新分配的缓存目录路径字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_app_cache_directory(const char* app_name);

// ========================================
// 系统信息
// ========================================

/**
 * @brief 获取操作系统名称
 * @return 新分配的操作系统名称字符串，调用者需使用 cckit_runtime_free_string 释放
 *
 * 可能的返回值：
 * - "Windows"
 * - "Linux"
 * - "macOS"
 * - "Unknown"
 */
CCKIT_API char* cckit_runtime_get_os_name(void);

/**
 * @brief 获取操作系统版本
 * @return 新分配的操作系统版本字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_os_version(void);

/**
 * @brief 获取系统架构
 * @return 新分配的系统架构字符串，调用者需使用 cckit_runtime_free_string 释放
 *
 * 可能的返回值：
 * - "x86_64"
 * - "x86"
 * - "arm64"
 * - "arm"
 * - "Unknown"
 */
CCKIT_API char* cckit_runtime_get_os_architecture(void);

/**
 * @brief 获取主机名
 * @return 新分配的主机名字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_host_name(void);

/**
 * @brief 获取当前用户名
 * @return 新分配的用户名字符串，调用者需使用 cckit_runtime_free_string 释放
 */
CCKIT_API char* cckit_runtime_get_user_name(void);

/**
 * @brief 获取处理器核心数
 * @return 处理器核心数，如果获取失败返回 0
 */
CCKIT_API int cckit_runtime_get_processor_count(void);

/**
 * @brief 获取总内存大小（字节）
 * @return 总内存大小，如果获取失败返回 0
 */
CCKIT_API size_t cckit_runtime_get_total_memory(void);

/**
 * @brief 获取可用内存大小（字节）
 * @return 可用内存大小，如果获取失败返回 0
 */
CCKIT_API size_t cckit_runtime_get_available_memory(void);

// ========================================
// 时间信息
// ========================================

/**
 * @brief 获取进程运行时间（毫秒）
 * @return 进程运行时间（毫秒），如果获取失败返回 0
 */
CCKIT_API uint64_t cckit_runtime_get_process_uptime(void);

/**
 * @brief 获取系统时间（Unix 时间戳，毫秒）
 * @return 系统时间（毫秒），如果获取失败返回 0
 */
CCKIT_API uint64_t cckit_runtime_get_system_time(void);

/**
 * @brief 获取单调时间（毫秒）
 * @return 单调时间（毫秒），如果获取失败返回 0
 *
 * 单调时间不受系统时间调整影响，适合用于计算时间间隔。
 */
CCKIT_API uint64_t cckit_runtime_get_monotonic_time(void);

// ========================================
// 内存管理
// ========================================

/**
 * @brief 释放由 cckit_runtime_* 函数分配的字符串
 * @param str 要释放的字符串
 */
CCKIT_API void cckit_runtime_free_string(char* str);

/**
 * @brief 释放由 cckit_runtime_get_path_env 分配的字符串数组
 * @param array 要释放的字符串数组
 * @param count 数组中的字符串数量
 */
CCKIT_API void cckit_runtime_free_string_array(char** array, size_t count);

#ifdef __cplusplus
}
#endif
