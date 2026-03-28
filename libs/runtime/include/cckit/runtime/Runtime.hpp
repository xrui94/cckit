// Runtime.hpp - 运行时工具 C++ 包装层
#pragma once

#include "cckit/runtime/capi/runtime.h"

#include <string>
#include <vector>
#include <map>
#include <cstdint>

namespace cckit::runtime
{

    // ========================================
    // 可执行文件路径
    // ========================================

    /**
     * @brief 获取当前可执行文件的完整路径
     * @return 可执行文件的完整路径，如果失败则返回空字符串
     */
    inline std::string getExecutablePath()
    {
        char* result = cckit_runtime_get_executable_path();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 获取当前可执行文件所在的目录
     * @return 可执行文件所在的目录路径，如果失败则返回空字符串
     */
    inline std::string getExecutableDir()
    {
        char* result = cckit_runtime_get_executable_dir();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    // ========================================
    // 动态库路径
    // ========================================

    /**
     * @brief 获取当前动态库的完整路径（包括文件名）
     *
     * 此函数确定其自身所在的动态链接库（DLL/so/dylib）的完整文件路径。
     *
     * @return 动态库的完整路径。如果失败则返回空字符串。
     */
    inline std::string getLibraryPath()
    {
        char* result = cckit_runtime_get_library_path();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 获取当前动态库所在的目录路径
     *
     * 此函数调用 getLibraryPath 获取完整路径，然后提取目录部分。
     *
     * @return 动态库所在的目录路径。如果失败则返回空字符串。
     */
    inline std::string getLibraryDir()
    {
        char* result = cckit_runtime_get_library_dir();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    // ========================================
    // 进程信息
    // ========================================

    /**
     * @brief 获取当前进程 ID
     * @return 进程 ID
     */
    inline int getProcessId()
    {
        return cckit_runtime_get_process_id();
    }

    /**
     * @brief 获取父进程 ID
     * @return 父进程 ID，如果获取失败返回 -1
     */
    inline int getParentProcessId()
    {
        return cckit_runtime_get_parent_process_id();
    }

    /**
     * @brief 获取当前进程名称
     * @return 进程名称，如果获取失败返回空字符串
     */
    inline std::string getProcessName()
    {
        char* result = cckit_runtime_get_process_name();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 获取命令行参数数量
     * @return 命令行参数数量
     */
    inline int getArgc()
    {
        return cckit_runtime_get_argc();
    }

    /**
     * @brief 获取命令行参数数组
     * @return 命令行参数数组
     */
    std::vector<std::string> getArgv();

    /**
     * @brief 获取完整的命令行字符串
     * @return 命令行字符串，如果获取失败返回空字符串
     */
    inline std::string getCommandLine()
    {
        char* result = cckit_runtime_get_command_line();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    // ========================================
    // 环境变量
    // ========================================

    /**
     * @brief 获取环境变量
     * @param name 环境变量名称
     * @return 环境变量值，如果不存在返回空字符串
     */
    inline std::string getEnv(const std::string& name)
    {
        char* result = cckit_runtime_get_env(name.c_str());
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 获取环境变量，如果不存在则返回默认值
     * @param name 环境变量名称
     * @param defaultValue 默认值
     * @return 环境变量值
     */
    inline std::string getEnvOrDefault(const std::string& name, const std::string& defaultValue)
    {
        char* result = cckit_runtime_get_env_or_default(name.c_str(), defaultValue.c_str());
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 设置环境变量
     * @param name 环境变量名称
     * @param value 环境变量值
     * @return true 如果设置成功
     */
    inline bool setEnv(const std::string& name, const std::string& value)
    {
        return cckit_runtime_set_env(name.c_str(), value.c_str());
    }

    /**
     * @brief 删除环境变量
     * @param name 环境变量名称
     * @return true 如果删除成功
     */
    inline bool unsetEnv(const std::string& name)
    {
        return cckit_runtime_unset_env(name.c_str());
    }

    /**
     * @brief 获取 PATH 环境变量中的路径列表
     * @return 路径列表
     */
    std::vector<std::string> getPathEnv();

    /**
     * @brief 获取所有环境变量
     * @return 环境变量映射表
     */
    std::map<std::string, std::string> getAllEnv();

    // ========================================
    // 工作目录
    // ========================================

    /**
     * @brief 获取当前工作目录
     * @return 当前目录路径，如果获取失败返回空字符串
     */
    inline std::string getCurrentWorkingDirectory()
    {
        char* result = cckit_runtime_get_current_working_directory();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 设置当前工作目录
     * @param path 新的工作目录路径
     * @return true 如果设置成功
     */
    inline bool setCurrentWorkingDirectory(const std::string& path)
    {
        return cckit_runtime_set_current_working_directory(path.c_str());
    }

    /**
     * @brief 获取临时目录
     * @return 临时目录路径，如果获取失败返回空字符串
     */
    inline std::string getTempDirectory()
    {
        char* result = cckit_runtime_get_temp_directory();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 创建临时目录
     * @param prefix 目录前缀
     * @return 临时目录路径，如果创建失败返回空字符串
     */
    inline std::string createTempDirectory(const std::string& prefix = "cckit_")
    {
        char* result = cckit_runtime_create_temp_directory(prefix.c_str());
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 创建临时文件
     * @param prefix 文件前缀
     * @return 临时文件路径，如果创建失败返回空字符串
     */
    inline std::string createTempFile(const std::string& prefix = "cckit_")
    {
        char* result = cckit_runtime_create_temp_file(prefix.c_str());
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    // ========================================
    // 标准路径
    // ========================================

    /**
     * @brief 获取用户主目录
     * @return 主目录路径，如果获取失败返回空字符串
     */
    inline std::string getHomeDirectory()
    {
        char* result = cckit_runtime_get_home_directory();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 获取用户配置目录
     * @return 配置目录路径，如果获取失败返回空字符串
     *
     * 平台差异：
     * - Windows: C:\Users\username\AppData\Roaming
     * - Linux: /home/username/.config
     * - macOS: /Users/username/Library/Application Support
     */
    inline std::string getUserConfigDirectory()
    {
        char* result = cckit_runtime_get_user_config_directory();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 获取用户数据目录
     * @return 数据目录路径，如果获取失败返回空字符串
     *
     * 平台差异：
     * - Windows: C:\Users\username\AppData\Local
     * - Linux: /home/username/.local/share
     * - macOS: /Users/username/Library/Application Support
     */
    inline std::string getUserDataDirectory()
    {
        char* result = cckit_runtime_get_user_data_directory();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 获取用户缓存目录
     * @return 缓存目录路径，如果获取失败返回空字符串
     *
     * 平台差异：
     * - Windows: C:\Users\username\AppData\Local\Temp
     * - Linux: /home/username/.cache
     * - macOS: /Users/username/Library/Caches
     */
    inline std::string getUserCacheDirectory()
    {
        char* result = cckit_runtime_get_user_cache_directory();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 获取应用程序配置目录
     * @param appName 应用程序名称
     * @return 配置目录路径，如果获取失败返回空字符串
     */
    inline std::string getAppConfigDirectory(const std::string& appName)
    {
        char* result = cckit_runtime_get_app_config_directory(appName.c_str());
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 获取应用程序数据目录
     * @param appName 应用程序名称
     * @return 数据目录路径，如果获取失败返回空字符串
     */
    inline std::string getAppDataDirectory(const std::string& appName)
    {
        char* result = cckit_runtime_get_app_data_directory(appName.c_str());
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 获取应用程序缓存目录
     * @param appName 应用程序名称
     * @return 缓存目录路径，如果获取失败返回空字符串
     */
    inline std::string getAppCacheDirectory(const std::string& appName)
    {
        char* result = cckit_runtime_get_app_cache_directory(appName.c_str());
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    // ========================================
    // 系统信息
    // ========================================

    /**
     * @brief 获取操作系统名称
     * @return 操作系统名称
     *
     * 可能的返回值：
     * - "Windows"
     * - "Linux"
     * - "macOS"
     * - "Unknown"
     */
    inline std::string getOSName()
    {
        char* result = cckit_runtime_get_os_name();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 获取操作系统版本
     * @return 操作系统版本，如果获取失败返回空字符串
     */
    inline std::string getOSVersion()
    {
        char* result = cckit_runtime_get_os_version();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 获取系统架构
     * @return 系统架构
     *
     * 可能的返回值：
     * - "x86_64"
     * - "x86"
     * - "arm64"
     * - "arm"
     * - "Unknown"
     */
    inline std::string getOSArchitecture()
    {
        char* result = cckit_runtime_get_os_architecture();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 获取主机名
     * @return 主机名，如果获取失败返回空字符串
     */
    inline std::string getHostName()
    {
        char* result = cckit_runtime_get_host_name();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 获取当前用户名
     * @return 用户名，如果获取失败返回空字符串
     */
    inline std::string getUserName()
    {
        char* result = cckit_runtime_get_user_name();
        std::string str(result ? result : "");
        cckit_runtime_free_string(result);
        return str;
    }

    /**
     * @brief 获取处理器核心数
     * @return 处理器核心数，如果获取失败返回 0
     */
    inline int getProcessorCount()
    {
        return cckit_runtime_get_processor_count();
    }

    /**
     * @brief 获取总内存大小（字节）
     * @return 总内存大小，如果获取失败返回 0
     */
    inline size_t getTotalMemory()
    {
        return cckit_runtime_get_total_memory();
    }

    /**
     * @brief 获取可用内存大小（字节）
     * @return 可用内存大小，如果获取失败返回 0
     */
    inline size_t getAvailableMemory()
    {
        return cckit_runtime_get_available_memory();
    }

    // ========================================
    // 时间信息
    // ========================================

    /**
     * @brief 获取进程运行时间（毫秒）
     * @return 进程运行时间（毫秒），如果获取失败返回 0
     */
    inline uint64_t getProcessUptime()
    {
        return cckit_runtime_get_process_uptime();
    }

    /**
     * @brief 获取系统时间（Unix 时间戳，毫秒）
     * @return 系统时间（毫秒），如果获取失败返回 0
     */
    inline uint64_t getSystemTime()
    {
        return cckit_runtime_get_system_time();
    }

    /**
     * @brief 获取单调时间（毫秒）
     * @return 单调时间（毫秒），如果获取失败返回 0
     *
     * 单调时间不受系统时间调整影响，适合用于计算时间间隔。
     */
    inline uint64_t getMonotonicTime()
    {
        return cckit_runtime_get_monotonic_time();
    }

} // namespace cckit::runtime
