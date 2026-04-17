// fs.h - 文件系统工具 C 接口
#pragma once

#include "../../export.h"

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    // ========================================
    // 路径查询
    // ========================================

    /**
     * @brief 检查路径是否存在
     * @param path 文件或目录路径
     * @return true 如果路径存在
     */
    CCKIT_API bool cckit_fs_exists(const char* path);

    /**
     * @brief 检查路径是否为常规文件
     * @param path 文件路径
     * @return true 如果路径存在且为常规文件
     */
    CCKIT_API bool cckit_fs_is_regular_file(const char* path);

    /**
     * @brief 检查路径是否为目录
     * @param path 目录路径
     * @return true 如果路径存在且为目录
     */
    CCKIT_API bool cckit_fs_is_directory(const char* path);

    /**
     * @brief 获取路径分隔符
     * @return 路径分隔符字符串（Windows 为 "\"，其他为 "/"）
     */
    CCKIT_API const char* cckit_fs_get_separator(void);

    /**
     * @brief 获取通用路径格式（使用 "/" 作为分隔符）
     * @param path 输入路径
     * @return 新分配的通用路径字符串，调用者需使用 cckit_fs_free_string 释放
     */
    CCKIT_API char* cckit_fs_get_generic_path(const char* path);

    /**
     * @brief 获取绝对路径
     * @param path 相对或绝对路径
     * @return 新分配的绝对路径字符串，调用者需使用 cckit_fs_free_string 释放
     */
    CCKIT_API char* cckit_fs_get_absolute_path(const char* path);

    /**
     * @brief 获取当前工作目录
     * @return 新分配的当前目录路径字符串，调用者需使用 cckit_fs_free_string 释放
     */
    CCKIT_API char* cckit_fs_get_current_directory(void);

    /**
     * @brief 获取父目录路径
     * @param path 文件或目录路径
     * @return 新分配的父目录路径字符串，调用者需使用 cckit_fs_free_string 释放
     */
    CCKIT_API char* cckit_fs_get_parent_path(const char* path);

    /**
     * @brief 获取文件名（包含扩展名）
     * @param path 文件路径
     * @return 新分配的文件名字符串，调用者需使用 cckit_fs_free_string 释放
     */
    CCKIT_API char* cckit_fs_get_file_name(const char* path);

    /**
     * @brief 获取文件名（不包含扩展名）
     * @param path 文件路径
     * @return 新分配的文件基础名字符串，调用者需使用 cckit_fs_free_string 释放
     */
    CCKIT_API char* cckit_fs_get_file_base(const char* path);

    /**
     * @brief 获取文件扩展名（包含点号）
     * @param path 文件路径
     * @return 新分配的扩展名字符串，调用者需使用 cckit_fs_free_string 释放
     */
    CCKIT_API char* cckit_fs_get_file_suffix(const char* path);

    /**
     * @brief 获取目录路径
     * @param path 文件或目录路径
     * @return 新分配的目录路径字符串，调用者需使用 cckit_fs_free_string 释放
     */
    CCKIT_API char* cckit_fs_get_directory_path(const char* path);

    /**
     * @brief 获取目录名（路径的最后一部分）
     * @param path 文件或目录路径
     * @return 新分配的目录名字符串，调用者需使用 cckit_fs_free_string 释放
     */
    CCKIT_API char* cckit_fs_get_directory_name(const char* path);

    /**
     * @brief 根据“基准文件”的绝对路径和指定的“相对路径”，计算相对路径的真实绝对路径
     *
     * @param baseFileAbsolute 基准文件的绝对路径 (例如: /home/user/project/config/settings.json)
     * @param relativePath     相对于基准文件的路径 (例如: ../data/image.png)
     * @return std::filesystem::path 目标的绝对路径
     */
    CCKIT_API char* cckit_fs_resolve_relative_path(const char* baseFileAbsolute, const char* relativePath);

    // ========================================
    // 文件操作
    // ========================================

    /**
     * @brief 复制文件
     * @param srcFilename 源文件路径
     * @param dstFilename 目标文件路径
     * @param createNewPath 是否创建目标路径的父目录
     * @return true 如果复制成功
     */
    CCKIT_API bool cckit_fs_copy_file(const char* srcFilename, const char* dstFilename, bool createNewPath);

    /**
     * @brief 删除文件
     * @param path 文件路径
     * @return true 如果删除成功
     */
    CCKIT_API bool cckit_fs_remove_file(const char* path);

    /**
     * @brief 获取文件大小
     * @param path 文件路径
     * @param outSize 输出：文件大小（字节）
     * @return true 如果获取成功
     */
    CCKIT_API bool cckit_fs_get_file_size(const char* path, size_t* outSize);

    // ========================================
    // 目录操作
    // ========================================

    /**
     * @brief 创建目录（如果不存在则创建）
     * @param path 目录路径
     * @return true 如果目录已存在或创建成功
     */
    CCKIT_API bool cckit_fs_ensure_path_exists(const char* path);

    /**
     * @brief 创建目录（递归创建）
     * @param path 目录路径
     * @return true 如果创建成功
     */
    CCKIT_API bool cckit_fs_create_directory(const char* path);

    /**
     * @brief 删除空目录
     * @param path 目录路径
     * @return true 如果删除成功
     */
    CCKIT_API bool cckit_fs_remove_directory(const char* path);

    /**
     * @brief 删除目录及其所有内容（递归）
     * @param path 目录路径
     * @return true 如果删除成功
     */
    CCKIT_API bool cckit_fs_remove_directory_all(const char* path);

    /**
     * @brief 列出目录中的文件
     * @param folder 目录路径
     * @param matchExtensions 匹配的扩展名列表（逗号分隔，如 ".txt,.csv"），nullptr 表示所有文件
     * @param outCount 输出：文件数量
     * @return 新分配的文件路径数组，调用者需使用 cckit_fs_free_string_array 释放
     */
    CCKIT_API char** cckit_fs_list_files(const char* folder, const char* matchExtensions, size_t* outCount);

    // ========================================
    // 内存管理
    // ========================================

    /**
     * @brief 释放由 cckit_fs_* 函数分配的字符串
     * @param str 要释放的字符串
     */
    CCKIT_API void cckit_fs_free_string(char* str);

    /**
     * @brief 释放由 cckit_fs_list_files 分配的字符串数组
     * @param array 要释放的字符串数组
     * @param count 数组中的字符串数量
     */
    CCKIT_API void cckit_fs_free_string_array(char** array, size_t count);

#ifdef __cplusplus
}
#endif
