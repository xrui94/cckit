// string.h - 字符串工具 C 接口
#pragma once

#include "../export.h"
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// 基础字符串操作
// ========================================

/**
 * @brief 检查字符串是否以指定前缀开头
 * @param str 要检查的字符串
 * @param prefix 要检查的前缀
 * @param ignoreCase 是否忽略大小写
 * @return true 如果字符串以指定前缀开头
 */
CCKIT_API bool cckit_string_starts_with(const char* str, const char* prefix, bool ignoreCase);

/**
 * @brief 检查字符串是否以指定后缀结尾
 * @param str 要检查的字符串
 * @param suffix 要检查的后缀
 * @param ignoreCase 是否忽略大小写
 * @return true 如果字符串以指定后缀结尾
 */
CCKIT_API bool cckit_string_ends_with(const char* str, const char* suffix, bool ignoreCase);

/**
 * @brief 去除字符串两端的指定字符
 * @param str 输入字符串
 * @param charsToRemove 要移除的字符集合
 * @param removeMiddleChars 是否同时移除中间的字符
 * @return 新分配的字符串，调用者需使用 cckit_string_free 释放
 */
CCKIT_API char* cckit_string_trim(const char* str, const char* charsToRemove, bool removeMiddleChars);

/**
 * @brief 将字符串转换为小写
 * @param str 输入字符串
 * @return 新分配的小写字符串，调用者需使用 cckit_string_free 释放
 */
CCKIT_API char* cckit_string_to_lower(const char* str);

/**
 * @brief 将字符串转换为大写
 * @param str 输入字符串
 * @return 新分配的大写字符串，调用者需使用 cckit_string_free 释放
 */
CCKIT_API char* cckit_string_to_upper(const char* str);

/**
 * @brief 比较两个字符串是否相等
 * @param s1 第一个字符串
 * @param s2 第二个字符串
 * @param ignoreCase 是否忽略大小写
 * @return true 如果两个字符串相等
 */
CCKIT_API bool cckit_string_equals(const char* s1, const char* s2, bool ignoreCase);

/**
 * @brief 分割字符串
 * @param str 输入字符串
 * @param delimiter 分隔符
 * @param mode 分割模式：0=保留所有，1=丢弃尾随空，2=跳过所有空
 * @param outCount 输出：分割后的字符串数量
 * @return 新分配的字符串数组，调用者需使用 cckit_string_free_array 释放
 */
CCKIT_API char** cckit_string_split(const char* str, const char* delimiter, int mode, size_t* outCount);

/**
 * @brief 检查字符串是否为有效的 UTF-8 编码
 * @param str 输入字符串
 * @return true 如果字符串是有效的 UTF-8 编码
 */
CCKIT_API bool cckit_string_is_valid_utf8(const char* str);

// ========================================
// 编码转换
// ========================================

/**
 * @brief 将字符串转换为 UTF-8 编码
 * @param str 输入字符串（假设为系统默认编码）
 * @return 新分配的 UTF-8 字符串，调用者需使用 cckit_string_free 释放
 */
CCKIT_API char* cckit_string_to_utf8(const char* str);

/**
 * @brief 将 UTF-8 字符串转换为系统默认编码
 * @param utf8Str 输入的 UTF-8 字符串
 * @return 新分配的系统编码字符串，调用者需使用 cckit_string_free 释放
 */
CCKIT_API char* cckit_string_from_utf8(const char* utf8Str);

/**
 * @brief 将字符串转换为 GBK 编码
 * @param str 输入字符串（假设为系统默认编码）
 * @return 新分配的 GBK 字符串，调用者需使用 cckit_string_free 释放
 */
CCKIT_API char* cckit_string_to_gbk(const char* str);

/**
 * @brief 将字符串转换为 GB2312 编码
 * @param str 输入字符串（假设为系统默认编码）
 * @return 新分配的 GB2312 字符串，调用者需使用 cckit_string_free 释放
 */
CCKIT_API char* cckit_string_to_gb2312(const char* str);

/**
 * @brief 将字符串转换为 GB18030 编码
 * @param str 输入字符串（假设为系统默认编码）
 * @return 新分配的 GB18030 字符串，调用者需使用 cckit_string_free 释放
 */
CCKIT_API char* cckit_string_to_gb18030(const char* str);

/**
 * @brief 从 GBK 编码转换为系统默认编码
 * @param gbkStr 输入的 GBK 字符串
 * @return 新分配的系统编码字符串，调用者需使用 cckit_string_free 释放
 */
CCKIT_API char* cckit_string_from_gbk(const char* gbkStr);

/**
 * @brief 从 GB2312 编码转换为系统默认编码
 * @param gb2312Str 输入的 GB2312 字符串
 * @return 新分配的系统编码字符串，调用者需使用 cckit_string_free 释放
 */
CCKIT_API char* cckit_string_from_gb2312(const char* gb2312Str);

/**
 * @brief 从 GB18030 编码转换为系统默认编码
 * @param gb18030Str 输入的 GB18030 字符串
 * @return 新分配的系统编码字符串，调用者需使用 cckit_string_free 释放
 */
CCKIT_API char* cckit_string_from_gb18030(const char* gb18030Str);

/**
 * @brief 通用编码转换函数
 * @param fromCode 源编码（UTF-8, GBK, GB2312, GB18030, 或系统默认编码）
 * @param toCode 目标编码
 * @param str 输入字符串
 * @return 新分配的转换后字符串，调用者需使用 cckit_string_free 释放
 */
CCKIT_API char* cckit_string_convert(const char* fromCode, const char* toCode, const char* str);

// ========================================
// 宽字符支持
// ========================================

/**
 * @brief 将 UTF-8 字符串转换为宽字符串（wchar_t*）
 * @param utf8Str 输入的 UTF-8 字符串
 * @param outWideChars 输出：宽字符数
 * @return 新分配的宽字符串，调用者需使用 cckit_string_free_wchars 释放
 */
CCKIT_API wchar_t* cckit_string_utf8_to_wchars(const char* utf8Str, size_t* outWideChars);

/**
 * @brief 将宽字符串（wchar_t*）转换为 UTF-8 字符串
 * @param wstr 输入的宽字符串
 * @return 新分配的 UTF-8 字符串，调用者需使用 cckit_string_free 释放
 */
CCKIT_API char* cckit_string_wchars_to_utf8(const wchar_t* wstr);

/**
 * @brief 释放宽字符串
 * @param wstr 要释放的宽字符串
 */
CCKIT_API void cckit_string_free_wchars(wchar_t* wstr);

// ========================================
// 内存管理
// ========================================

/**
 * @brief 释放由 cckit_string_* 函数分配的字符串
 * @param str 要释放的字符串
 */
CCKIT_API void cckit_string_free(char* str);

/**
 * @brief 释放由 cckit_string_split 分配的字符串数组
 * @param array 要释放的字符串数组
 * @param count 数组中的字符串数量
 */
CCKIT_API void cckit_string_free_array(char** array, size_t count);

#ifdef __cplusplus
}
#endif
