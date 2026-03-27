// string.cpp - 字符串工具实现
#include "cckit/string/String.hpp"
#include "cckit/export.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <vector>
#include <sstream>

#ifdef _WIN32
    #include <Windows.h>
    // Windows 平台使用 _stricmp 代替 strcasecmp
    #define strcasecmp _stricmp
    // Windows 平台使用 _strdup 代替 strdup
    #define strdup _strdup
#endif

namespace cckit::string
{

    std::vector<std::string> split(const std::string& str, const std::string& delimiter, SplitMode mode)
    {
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

}


extern "C" {

    // ========================================
    // 辅助函数
    // ========================================

    static bool char_equals_ignore_case(char a, char b) {
        return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
    }

    static bool string_equals_ignore_case(const char* s1, const char* s2) {
        if (!s1 || !s2) return s1 == s2;
        while (*s1 && *s2) {
            if (!char_equals_ignore_case(*s1, *s2)) return false;
            s1++;
            s2++;
        }
        return *s1 == *s2;
    }

    // ========================================
    // 基础字符串操作实现
    // ========================================

    bool cckit_string_starts_with(const char* str, const char* prefix, bool ignoreCase) {
        if (!str || !prefix) return false;
        size_t strLen = strlen(str);
        size_t prefixLen = strlen(prefix);
        if (prefixLen > strLen) return false;

        if (ignoreCase) {
            for (size_t i = 0; i < prefixLen; ++i) {
                if (!char_equals_ignore_case(str[i], prefix[i])) return false;
            }
            return true;
        } else {
            return strncmp(str, prefix, prefixLen) == 0;
        }
    }

    bool cckit_string_ends_with(const char* str, const char* suffix, bool ignoreCase) {
        if (!str || !suffix) return false;
        size_t strLen = strlen(str);
        size_t suffixLen = strlen(suffix);
        if (suffixLen > strLen) return false;

        const char* strEnd = str + strLen - suffixLen;
        if (ignoreCase) {
            for (size_t i = 0; i < suffixLen; ++i) {
                if (!char_equals_ignore_case(strEnd[i], suffix[i])) return false;
            }
            return true;
        } else {
            return strcmp(strEnd, suffix) == 0;
        }
    }

    char* cckit_string_trim(const char* str, const char* charsToRemove, bool removeMiddleChars) {
        if (!str) return nullptr;

        std::string result(str);

        // 移除前导字符
        size_t start = result.find_first_not_of(charsToRemove);
        if (start == std::string::npos) {
            // 全部都是要移除的字符
            return strdup("");
        }
        result = result.substr(start);

        // 移除尾随字符
        size_t end = result.find_last_not_of(charsToRemove);
        if (end != std::string::npos) {
            result = result.substr(0, end + 1);
        }

        // 移除中间字符
        if (removeMiddleChars) {
            std::string temp;
            for (char c : result) {
                if (charsToRemove && strchr(charsToRemove, c) == nullptr) {
                    temp += c;
                }
            }
            result = temp;
        }

        return strdup(result.c_str());
    }

    char* cckit_string_to_lower(const char* str) {
        if (!str) return nullptr;

        std::string result(str);
        std::transform(result.begin(), result.end(), result.begin(),
                    [](unsigned char c) { return std::tolower(c); });

        return strdup(result.c_str());
    }

    char* cckit_string_to_upper(const char* str) {
        if (!str) return nullptr;

        std::string result(str);
        std::transform(result.begin(), result.end(), result.begin(),
                    [](unsigned char c) { return std::toupper(c); });

        return strdup(result.c_str());
    }

    bool cckit_string_equals(const char* s1, const char* s2, bool ignoreCase) {
        if (!s1 || !s2) return s1 == s2;

        if (ignoreCase) {
            return string_equals_ignore_case(s1, s2);
        } else {
            return strcmp(s1, s2) == 0;
        }
    }

    char** cckit_string_split(const char* str, const char* delimiter, int mode, size_t* outCount) {
        if (!str || !delimiter || !outCount) {
            if (outCount) *outCount = 0;
            return nullptr;
        }

        std::string delim(delimiter);

        // 特殊处理：空分隔符，返回整个字符串作为单个 token
        if (delim.empty()) {
            *outCount = 1;
            char** result = static_cast<char**>(malloc(sizeof(char*)));
            if (!result) {
                *outCount = 0;
                return nullptr;
            }
            result[0] = strdup(str);
            return result;
        }

        std::vector<std::string> tokens;
        std::string s(str);
        size_t start = 0;
        size_t end = s.find(delim);

        while (end != std::string::npos) {
            std::string token = s.substr(start, end - start);
            tokens.push_back(token);
            start = end + delim.length();
            end = s.find(delim, start);
        }
        tokens.push_back(s.substr(start));

        // 根据模式处理空 token
        std::vector<std::string> filtered;
        for (size_t i = 0; i < tokens.size(); ++i) {
            const std::string& token = tokens[i];
            bool isEmpty = token.empty();

            if (mode == 0) { // KeepAll
                filtered.push_back(token);
            } else if (mode == 1) { // TrimTrailing
                if (i < tokens.size() - 1 || !isEmpty) {
                    filtered.push_back(token);
                }
            } else if (mode == 2) { // SkipAll
                if (!isEmpty) {
                    filtered.push_back(token);
                }
            }
        }

        // 分配结果数组
        *outCount = filtered.size();
        char** result = static_cast<char**>(malloc((*outCount) * sizeof(char*)));
        if (!result) {
            *outCount = 0;
            return nullptr;
        }

        for (size_t i = 0; i < *outCount; ++i) {
            result[i] = strdup(filtered[i].c_str());
        }

        return result;
    }

    bool cckit_string_is_valid_utf8(const char* str) {
        if (!str) return false;

        const unsigned char* bytes = reinterpret_cast<const unsigned char*>(str);
        while (*bytes) {
            if ((*bytes & 0x80) == 0x00) {
                // 1-byte sequence (0xxxxxxx)
                bytes++;
            } else if ((*bytes & 0xE0) == 0xC0) {
                // 2-byte sequence (110xxxxx 10xxxxxx)
                if ((bytes[1] & 0xC0) != 0x80) return false;
                bytes += 2;
            } else if ((*bytes & 0xF0) == 0xE0) {
                // 3-byte sequence (1110xxxx 10xxxxxx 10xxxxxx)
                if ((bytes[1] & 0xC0) != 0x80 || (bytes[2] & 0xC0) != 0x80) return false;
                bytes += 3;
            } else if ((*bytes & 0xF8) == 0xF0) {
                // 4-byte sequence (11110xxx 10xxxxxx 10xxxxxx 10xxxxxx)
                if ((bytes[1] & 0xC0) != 0x80 || (bytes[2] & 0xC0) != 0x80 || (bytes[3] & 0xC0) != 0x80) return false;
                bytes += 4;
            } else {
                return false;
            }
        }
        return true;
    }

    // ========================================
    // 编码转换实现
    // ========================================

    #ifdef _WIN32

        // Windows 编码转换辅助函数
        static char* convertEncoding(UINT fromCode, UINT toCode, const char* str) {
            if (!str) return nullptr;

            // 先转换为宽字符
            int wlen = MultiByteToWideChar(fromCode, 0, str, -1, nullptr, 0);
            if (wlen <= 0) return strdup(str);

            wchar_t* wstr = static_cast<wchar_t*>(malloc(wlen * sizeof(wchar_t)));
            if (!wstr) return strdup(str);

            MultiByteToWideChar(fromCode, 0, str, -1, wstr, wlen);

            // 再转换为目标编码
            int targetLen = WideCharToMultiByte(toCode, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
            if (targetLen <= 0) {
                free(wstr);
                return strdup(str);
            }

            char* targetStr = static_cast<char*>(malloc(targetLen));
            if (!targetStr) {
                free(wstr);
                return strdup(str);
            }

            WideCharToMultiByte(toCode, 0, wstr, -1, targetStr, targetLen, nullptr, nullptr);

            free(wstr);
            return targetStr;
        }

        // 获取编码代码页
        static UINT getEncodingCodePage(const char* encoding) {
            if (!encoding) return CP_ACP;

            if (strcasecmp(encoding, "UTF-8") == 0 || strcasecmp(encoding, "UTF8") == 0) {
                return CP_UTF8;
            } else if (strcasecmp(encoding, "GBK") == 0) {
                return 936;
            } else if (strcasecmp(encoding, "GB2312") == 0) {
                return 936; // GB2312 是 GBK 的子集
            } else if (strcasecmp(encoding, "GB18030") == 0) {
                return 54936;
            } else if (strcasecmp(encoding, "ANSI") == 0 || strcasecmp(encoding, "SYSTEM") == 0) {
                return CP_ACP;
            }

            return CP_ACP; // 默认使用系统编码
        }

    #else

        // Linux/Mac 编码转换辅助函数（使用 iconv）
        #include <iconv.h>

        static char* convertEncoding(const char* fromCode, const char* toCode, const char* str) {
            if (!str) return nullptr;

            // 如果源编码和目标编码相同，直接复制
            if (strcasecmp(fromCode, toCode) == 0) {
                return strdup(str);
            }

            iconv_t cd = iconv_open(toCode, fromCode);
            if (cd == (iconv_t)-1) {
                return strdup(str);
            }

            size_t inbytesleft = strlen(str);
            size_t outbytesleft = inbytesleft * 4; // 预留足够空间
            char* outbuf = static_cast<char*>(malloc(outbytesleft + 1));
            if (!outbuf) {
                iconv_close(cd);
                return strdup(str);
            }

            char* inptr = const_cast<char*>(str);
            char* outptr = outbuf;

            size_t result = iconv(cd, &inptr, &inbytesleft, &outptr, &outbytesleft);
            iconv_close(cd);

            if (result == (size_t)-1) {
                free(outbuf);
                return strdup(str);
            }

            *outptr = '\0';
            return outbuf;
        }

        // 获取标准编码名称
        static const char* getStandardEncodingName(const char* encoding) {
            if (!encoding) return "UTF-8";

            if (strcasecmp(encoding, "UTF-8") == 0 || strcasecmp(encoding, "UTF8") == 0) {
                return "UTF-8";
            } else if (strcasecmp(encoding, "GBK") == 0) {
                return "GBK";
            } else if (strcasecmp(encoding, "GB2312") == 0) {
                return "GB2312";
            } else if (strcasecmp(encoding, "GB18030") == 0) {
                return "GB18030";
            } else if (strcasecmp(encoding, "ANSI") == 0 || strcasecmp(encoding, "SYSTEM") == 0) {
                return "UTF-8"; // Linux/Mac 默认使用 UTF-8
            }

            return "UTF-8"; // 默认使用 UTF-8
        }

    #endif

    char* cckit_string_to_utf8(const char* str) {
        if (!str) return nullptr;

    #ifdef _WIN32
        return convertEncoding(CP_ACP, CP_UTF8, str);
    #else
        return convertEncoding("UTF-8", "UTF-8", str); // Linux/Mac 假设已经是 UTF-8
    #endif
    }

    char* cckit_string_from_utf8(const char* utf8Str) {
        if (!utf8Str) return nullptr;

    #ifdef _WIN32
        return convertEncoding(CP_UTF8, CP_ACP, utf8Str);
    #else
        return strdup(utf8Str); // Linux/Mac 假设已经是 UTF-8
    #endif
    }

    char* cckit_string_to_gbk(const char* str) {
        if (!str) return nullptr;

    #ifdef _WIN32
        return convertEncoding(CP_ACP, 936, str);
    #else
        return convertEncoding("UTF-8", "GBK", str);
    #endif
    }

    char* cckit_string_to_gb2312(const char* str) {
        if (!str) return nullptr;

    #ifdef _WIN32
        return convertEncoding(CP_ACP, 936, str); // GB2312 使用 GBK 代码页
    #else
        return convertEncoding("UTF-8", "GB2312", str);
    #endif
    }

    char* cckit_string_to_gb18030(const char* str) {
        if (!str) return nullptr;

    #ifdef _WIN32
        return convertEncoding(CP_ACP, 54936, str);
    #else
        return convertEncoding("UTF-8", "GB18030", str);
    #endif
    }

    char* cckit_string_from_gbk(const char* gbkStr) {
        if (!gbkStr) return nullptr;

    #ifdef _WIN32
        return convertEncoding(936, CP_ACP, gbkStr);
    #else
        return convertEncoding("GBK", "UTF-8", gbkStr);
    #endif
    }

    char* cckit_string_from_gb2312(const char* gb2312Str) {
        if (!gb2312Str) return nullptr;

    #ifdef _WIN32
        return convertEncoding(936, CP_ACP, gb2312Str);
    #else
        return convertEncoding("GB2312", "UTF-8", gb2312Str);
    #endif
    }

    char* cckit_string_from_gb18030(const char* gb18030Str) {
        if (!gb18030Str) return nullptr;

    #ifdef _WIN32
        return convertEncoding(54936, CP_ACP, gb18030Str);
    #else
        return convertEncoding("GB18030", "UTF-8", gb18030Str);
    #endif
    }

    char* cckit_string_convert(const char* fromCode, const char* toCode, const char* str) {
        if (!str) return nullptr;
        if (!fromCode || !toCode) return strdup(str);

    #ifdef _WIN32
        UINT fromCP = getEncodingCodePage(fromCode);
        UINT toCP = getEncodingCodePage(toCode);
        return convertEncoding(fromCP, toCP, str);
    #else
        const char* fromEnc = getStandardEncodingName(fromCode);
        const char* toEnc = getStandardEncodingName(toCode);
        return convertEncoding(fromEnc, toEnc, str);
    #endif
    }

    // ========================================
    // 宽字符支持实现
    // ========================================

    wchar_t* cckit_string_utf8_to_wchars(const char* utf8Str, size_t* outWideChars) {
        if (!utf8Str) {
            if (outWideChars) *outWideChars = 0;
            return nullptr;
        }

        if (outWideChars) *outWideChars = 0;

    #ifdef _WIN32
        int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, nullptr, 0);
        if (wlen <= 0) return nullptr;

        wchar_t* wstr = static_cast<wchar_t*>(malloc(wlen * sizeof(wchar_t)));
        if (!wstr) return nullptr;

        MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, wstr, wlen);

        if (outWideChars) *outWideChars = wlen - 1; // 不包括 null 终止符
        return wstr;
    #else
        size_t len = strlen(utf8Str);
        size_t wlen = mbstowcs(nullptr, utf8Str, 0);
        if (wlen == (size_t)-1) return nullptr;

        wchar_t* wstr = static_cast<wchar_t*>(malloc((wlen + 1) * sizeof(wchar_t)));
        if (!wstr) return nullptr;

        mbstowcs(wstr, utf8Str, wlen + 1);

        if (outWideChars) *outWideChars = wlen;
        return wstr;
    #endif
    }

    char* cckit_string_wchars_to_utf8(const wchar_t* wstr) {
        if (!wstr) return nullptr;

    #ifdef _WIN32
        int utf8len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
        if (utf8len <= 0) return nullptr;

        char* utf8str = static_cast<char*>(malloc(utf8len));
        if (!utf8str) return nullptr;

        WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8str, utf8len, nullptr, nullptr);

        return utf8str;
    #else
        size_t utf8len = wcstombs(nullptr, wstr, 0);
        if (utf8len == (size_t)-1) return nullptr;

        char* utf8str = static_cast<char*>(malloc(utf8len + 1));
        if (!utf8str) return nullptr;

        wcstombs(utf8str, wstr, utf8len + 1);

        return utf8str;
    #endif
    }

    void cckit_string_free_wchars(wchar_t* wstr) {
        if (wstr) {
            free(wstr);
        }
    }

    // ========================================
    // 内存管理实现
    // ========================================

    void cckit_string_free(char* str) {
        if (str) {
            free(str);
        }
    }

    void cckit_string_free_array(char** array, size_t count) {
        if (array) {
            for (size_t i = 0; i < count; ++i) {
                if (array[i]) {
                    free(array[i]);
                }
            }
            free(array);
        }
    }

} // extern "C"
