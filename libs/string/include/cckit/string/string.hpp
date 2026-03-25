// string.hpp - 字符串工具 C++ 包装层
#pragma once

#include "string.h"
#include <string>
#include <vector>
#include <optional>

namespace cckit {
namespace string {

enum class SplitMode {
    KeepAll = 0,        // 保留所有空 token（包括尾随）
    TrimTrailing = 1,   // 保留前导/中间空，丢弃尾随空
    SkipAll = 2         // 跳过所有空 token
};

// ========================================
// 基础字符串操作
// ========================================

inline bool startsWith(const std::string& str, const std::string& prefix, bool ignoreCase = false) {
    return cckit_string_starts_with(str.c_str(), prefix.c_str(), ignoreCase);
}

inline bool endsWith(const std::string& str, const std::string& suffix, bool ignoreCase = false) {
    return cckit_string_ends_with(str.c_str(), suffix.c_str(), ignoreCase);
}

inline std::string trim(const std::string& str, const std::string& charsToRemove = " ", bool removeMiddleChars = false) {
    char* result = cckit_string_trim(str.c_str(), charsToRemove.c_str(), removeMiddleChars);
    std::string resultStr(result ? result : "");
    cckit_string_free(result);
    return resultStr;
}

inline std::string toLower(const std::string& str) {
    char* result = cckit_string_to_lower(str.c_str());
    std::string resultStr(result ? result : "");
    cckit_string_free(result);
    return resultStr;
}

inline std::string toUpper(const std::string& str) {
    char* result = cckit_string_to_upper(str.c_str());
    std::string resultStr(result ? result : "");
    cckit_string_free(result);
    return resultStr;
}

inline bool equals(const std::string& s1, const std::string& s2, bool ignoreCase = false) {
    return cckit_string_equals(s1.c_str(), s2.c_str(), ignoreCase);
}

inline std::vector<std::string> split(const std::string& str, const std::string& delimiter, SplitMode mode = SplitMode::TrimTrailing) {
    size_t count = 0;
    char** array = cckit_string_split(str.c_str(), delimiter.c_str(), static_cast<int>(mode), &count);

    std::vector<std::string> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        result.push_back(array[i] ? array[i] : "");
    }

    cckit_string_free_array(array, count);
    return result;
}

inline bool isValidUtf8(const std::string& str) {
    return cckit_string_is_valid_utf8(str.c_str());
}

// ========================================
// 编码转换
// ========================================

inline std::string toUtf8(const std::string& str) {
    char* result = cckit_string_to_utf8(str.c_str());
    std::string resultStr(result ? result : "");
    cckit_string_free(result);
    return resultStr;
}

inline std::string fromUtf8(const std::string& utf8Str) {
    char* result = cckit_string_from_utf8(utf8Str.c_str());
    std::string resultStr(result ? result : "");
    cckit_string_free(result);
    return resultStr;
}

inline std::string toGbk(const std::string& str) {
    char* result = cckit_string_to_gbk(str.c_str());
    std::string resultStr(result ? result : "");
    cckit_string_free(result);
    return resultStr;
}

inline std::string toGb2312(const std::string& str) {
    char* result = cckit_string_to_gb2312(str.c_str());
    std::string resultStr(result ? result : "");
    cckit_string_free(result);
    return resultStr;
}

inline std::string toGb18030(const std::string& str) {
    char* result = cckit_string_to_gb18030(str.c_str());
    std::string resultStr(result ? result : "");
    cckit_string_free(result);
    return resultStr;
}

inline std::string fromGbk(const std::string& gbkStr) {
    char* result = cckit_string_from_gbk(gbkStr.c_str());
    std::string resultStr(result ? result : "");
    cckit_string_free(result);
    return resultStr;
}

inline std::string fromGb2312(const std::string& gb2312Str) {
    char* result = cckit_string_from_gb2312(gb2312Str.c_str());
    std::string resultStr(result ? result : "");
    cckit_string_free(result);
    return resultStr;
}

inline std::string fromGb18030(const std::string& gb18030Str) {
    char* result = cckit_string_from_gb18030(gb18030Str.c_str());
    std::string resultStr(result ? result : "");
    cckit_string_free(result);
    return resultStr;
}

inline std::string convert(const std::string& str, const std::string& fromCode, const std::string& toCode) {
    char* result = cckit_string_convert(fromCode.c_str(), toCode.c_str(), str.c_str());
    std::string resultStr(result ? result : "");
    cckit_string_free(result);
    return resultStr;
}

// ========================================
// 宽字符支持
// ========================================

inline std::wstring utf8ToWchars(const std::string& utf8Str) {
    size_t wideChars = 0;
    wchar_t* wstr = cckit_string_utf8_to_wchars(utf8Str.c_str(), &wideChars);
    if (!wstr) return std::wstring();

    std::wstring result(wstr, wideChars);
    cckit_string_free_wchars(wstr);
    return result;
}

inline std::string wcharsToUtf8(const std::wstring& wstr) {
    char* result = cckit_string_wchars_to_utf8(wstr.c_str());
    std::string resultStr(result ? result : "");
    cckit_string_free(result);
    return resultStr;
}

// 便捷类型别名
using WideString = std::wstring;

} // namespace string
} // namespace cckit
