// fs.cpp - 文件系统工具实现（支持多后端）
#include "cckit/fs/fs.hpp"
#include "cckit/export.h"

#include <cstring>
#include <algorithm>
#include <set>

#ifdef _WIN32
    // Windows 平台使用 _strdup 代替 strdup
    #define strdup _strdup
#endif

// 根据后端选择不同的头文件
#ifdef CCKIT_USE_GHC_FS
    #include <ghc/filesystem.hpp>
    namespace fs = ghc::filesystem;
#else
    #include <filesystem>
    namespace fs = std::filesystem;
#endif


namespace cckit::fs
{

    std::vector<std::string> listFiles(const std::string& folder, const std::set<std::string>& matchExtensions) {
        // 将扩展名集合转换为逗号分隔的字符串
        std::string extensionsStr;
        if (!matchExtensions.empty()) {
            for (const auto& ext : matchExtensions) {
                if (!extensionsStr.empty()) {
                    extensionsStr += ",";
                }
                extensionsStr += ext;
            }
        }

        size_t count = 0;
        char** array = cckit_fs_list_files(folder.c_str(), extensionsStr.empty() ? nullptr : extensionsStr.c_str(), &count);

        std::vector<std::string> result;
        result.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            result.push_back(array[i] ? array[i] : "");
        }

        cckit_fs_free_string_array(array, count);
        return result;
    }

} // namespace cckit::fs

extern "C" {

    // ========================================
    // 路径查询实现
    // ========================================

    bool cckit_fs_exists(const char* path) {
        if (!path) return false;
        try {
#ifdef CCKIT_USE_GHC_FS
            std::error_code ec;
            return fs::exists(path, ec);
#else
            return fs::exists(path);
#endif
        } catch (...) {
            return false;
        }
    }

    bool cckit_fs_is_regular_file(const char* path) {
        if (!path) return false;
        try {
#ifdef CCKIT_USE_GHC_FS
            std::error_code ec;
            return fs::is_regular_file(path, ec);
#else
            return fs::is_regular_file(path);
#endif
        } catch (...) {
            return false;
        }
    }

    bool cckit_fs_is_directory(const char* path) {
        if (!path) return false;
        try {
#ifdef CCKIT_USE_GHC_FS
            std::error_code ec;
            return fs::is_directory(path, ec);
#else
            return fs::is_directory(path);
#endif
        } catch (...) {
            return false;
        }
    }

    const char* cckit_fs_get_separator(void) {
    #if defined(_WIN32) || defined(_WIN64)
        return "\\";
    #else
        return "/";
    #endif
    }

    char* cckit_fs_get_generic_path(const char* path) {
        if (!path) return nullptr;
        try {
            fs::path p(path);
            std::string result = p.generic_string();
            return strdup(result.c_str());
        } catch (...) {
            return nullptr;
        }
    }

    char* cckit_fs_get_absolute_path(const char* path) {
        if (!path) return nullptr;
        try {
#ifdef CCKIT_USE_GHC_FS
            std::error_code ec;
            fs::path p = fs::absolute(path, ec);
#else
            fs::path p = fs::absolute(path);
#endif
            std::string result = p.generic_string();
            return strdup(result.c_str());
        }
        catch (...) {
            return nullptr;
        }
    }

    char* cckit_fs_get_current_directory(void) {
        try {
#ifdef CCKIT_USE_GHC_FS
            std::error_code ec;
            fs::path p = fs::current_path(ec);
#else
            fs::path p = fs::current_path();
#endif
            std::string result = p.generic_string();
            return strdup(result.c_str());
        }
        catch (...) {
            return nullptr;
        }
    }

    char* cckit_fs_get_parent_path(const char* path) {
        if (!path) return nullptr;
        try {
            fs::path p(path);
            fs::path parent = p.parent_path();
            std::string result = parent.generic_string();
            return strdup(result.c_str());
        }
        catch (...) {
            return nullptr;
        }
    }

    char* cckit_fs_get_file_name(const char* path) {
        if (!path) return nullptr;
        try {
            fs::path p(path);
            std::string result = p.filename().generic_string();
            return strdup(result.c_str());
        }
        catch (...) {
            return nullptr;
        }
    }

    char* cckit_fs_get_file_base(const char* path) {
        if (!path) return nullptr;
        try {
            fs::path p(path);
            std::string result = p.stem().generic_string();
            return strdup(result.c_str());
        }
        catch (...) {
            return nullptr;
        }
    }

    char* cckit_fs_get_file_suffix(const char* path) {
        if (!path) return nullptr;
        try {
            fs::path p(path);
            std::string result = p.extension().generic_string();
            return strdup(result.c_str());
        }
        catch (...) {
            return nullptr;
        }
    }

    // 规范化路径（去除多余的 ./ 或 ../，但不检查磁盘存在性）
    char* cckit_fs_get_directory_path(const char* path) {
        if (!path || *path == '\0') return nullptr;

        try {
            fs::path p(path);
            fs::path normalized_p = p.lexically_normal();
            fs::path dir_path;

            // --- 核心逻辑修改开始 ---

            bool is_dir = false;

            // 1. 优先：询问操作系统"你是个目录吗？"（这是最准的）
            // 注意：这需要访问磁盘（stat系统调用），如果路径不存在会返回 false
#ifdef CCKIT_USE_GHC_FS
            std::error_code ec;
            if (fs::is_directory(p, ec)) {
                is_dir = true;
            }
#else
            std::error_code ec;
            if (fs::is_directory(p, ec)) {
                is_dir = true;
            }
#endif

            // 2. 兜底：如果磁盘上找不到（可能是不存在的新路径），
            // 但用户显式加了 '/' (如 "new_dir/")，我们也认为它是目录
            if (!is_dir) {
                std::string original_str = p.generic_string();
                if (!original_str.empty() && (original_str.back() == '/' || original_str.back() == '\\')) {
                    is_dir = true;
                }
            }

            // 3. 根据判断结果决定返回值
            if (is_dir) {
                // 是目录：返回自身
                dir_path = normalized_p;
            }
            else {
                // 是文件（或看起来像文件）：返回父目录
                dir_path = normalized_p.parent_path();
            }

            // --- 核心逻辑修改结束 ---

            // 边界情况：如果是 "file.txt"，parent_path 为空，返回当前目录 "."
            if (dir_path.empty()) {
                return strdup(".");
            }

            // 统一返回正斜杠格式
            std::string result = dir_path.generic_string();
            return strdup(result.c_str());

        }
        catch (...) {
            return nullptr;
        }
    }

    char* cckit_fs_get_directory_name(const char* path) {
        // 1. 获取完整路径 (拿到了 "path/to/dir/")
        char* dir_path_str = cckit_fs_get_directory_path(path);

        if (!dir_path_str) {
            return nullptr;
        }

        try {
            fs::path p(dir_path_str);

            // ==========================================
            // 【核心修复】手动去除末尾斜杠
            // ==========================================
            // lexically_normal() 不会去掉末尾的 '/'，所以我们要自己动手
            // 只要路径不是根目录 "/"，且末尾是分隔符，就把它去掉
            std::string p_str = p.generic_string(); // 统一转为正斜杠字符串处理

            // 只要长度大于1，且最后一个字符是 '/'，就截断
            // (保留长度1是为了处理根目录 "/" 的情况)
            if (p_str.length() > 1 && p_str.back() == '/') {
                p_str.pop_back();
            }

            fs::path clean_p(p_str);

            // 现在 clean_p 是 "path/to/dir"，filename() 就能正确返回 "dir" 了
            std::string name = clean_p.filename().generic_string();

            // ==========================================

            if (name.empty()) {
                // 处理根目录 "/" 的情况
                if (clean_p.has_root_path() && clean_p == clean_p.root_path()) {
                    name = "/";
                }
                else {
                    name = ".";
                }
            }

            char* result = strdup(name.c_str());

            // 释放底层分配的内存
            free(dir_path_str);

            return result;

        }
        catch (...) {
            free(dir_path_str);
            return nullptr;
        }
    }

    // ========================================
    // 文件操作实现
    // ========================================

    bool cckit_fs_copy_file(const char* srcFilename, const char* dstFilename, bool createNewPath) {
        if (!srcFilename || !dstFilename) return false;
        try {
            if (createNewPath) {
                fs::path dstPath(dstFilename);
                if (dstPath.has_parent_path()) {
#ifdef CCKIT_USE_GHC_FS
                    std::error_code ec;
                    fs::create_directories(dstPath.parent_path(), ec);
#else
                    fs::create_directories(dstPath.parent_path());
#endif
                }
            }
#ifdef CCKIT_USE_GHC_FS
            std::error_code ec;
            fs::copy_file(srcFilename, dstFilename, fs::copy_options::overwrite_existing, ec);
            return !ec;
#else
            fs::copy_file(srcFilename, dstFilename, fs::copy_options::overwrite_existing);
            return true;
#endif
        }
        catch (...) {
            return false;
        }
    }

    bool cckit_fs_remove_file(const char* path) {
        if (!path) return false;
        try {
#ifdef CCKIT_USE_GHC_FS
            std::error_code ec;
            return fs::remove(path, ec);
#else
            return fs::remove(path);
#endif
        }
        catch (...) {
            return false;
        }
    }

    bool cckit_fs_get_file_size(const char* path, size_t* outSize) {
        if (!path || !outSize) return false;
        try {
#ifdef CCKIT_USE_GHC_FS
            std::error_code ec;
            *outSize = fs::file_size(path, ec);
            return !ec;
#else
            * outSize = fs::file_size(path);
            return true;
#endif
        }
        catch (...) {
            return false;
        }
    }

    // ========================================
    // 目录操作实现
    // ========================================

    bool cckit_fs_ensure_path_exists(const char* path) {
        if (!path) return false;
        try {
#ifdef CCKIT_USE_GHC_FS
            std::error_code ec;
            if (fs::exists(path, ec)) {
                return fs::is_directory(path, ec);
            }
            return fs::create_directories(path, ec);
#else
            if (fs::exists(path)) {
                return fs::is_directory(path);
            }
            return fs::create_directories(path);
#endif
        }
        catch (...) {
            return false;
        }
    }

    bool cckit_fs_create_directory(const char* path) {
        if (!path) return false;
        try {
#ifdef CCKIT_USE_GHC_FS
            std::error_code ec;
            return fs::create_directories(path, ec);
#else
            return fs::create_directories(path);
#endif
        }
        catch (...) {
            return false;
        }
    }

    bool cckit_fs_remove_directory(const char* path) {
        if (!path) return false;
        try {
#ifdef CCKIT_USE_GHC_FS
            std::error_code ec;
            return fs::remove(path, ec);
#else
            return fs::remove(path);
#endif
        }
        catch (...) {
            return false;
        }
    }

    bool cckit_fs_remove_directory_all(const char* path) {
        if (!path) return false;
        try {
#ifdef CCKIT_USE_GHC_FS
            std::error_code ec;
            return fs::remove_all(path, ec) > 0;
#else
            return fs::remove_all(path) > 0;
#endif
        }
        catch (...) {
            return false;
        }
    }

    char** cckit_fs_list_files(const char* folder, const char* matchExtensions, size_t* outCount) {
        if (!folder || !outCount) {
            if (outCount) *outCount = 0;
            return nullptr;
        }

        try {
#ifdef CCKIT_USE_GHC_FS
            std::error_code ec;
            if (!fs::is_directory(folder, ec)) {
                *outCount = 0;
                return nullptr;
            }
#else
            if (!fs::is_directory(folder)) {
                *outCount = 0;
                return nullptr;
            }
#endif

            // 解析匹配的扩展名
            std::set<std::string> extensions;
            if (matchExtensions) {
                std::string extStr(matchExtensions);
                size_t start = 0;
                size_t end = extStr.find(',');
                while (end != std::string::npos) {
                    std::string ext = extStr.substr(start, end - start);
                    if (!ext.empty()) {
                        extensions.insert(ext);
                    }
                    start = end + 1;
                    end = extStr.find(',', start);
                }
                std::string lastExt = extStr.substr(start);
                if (!lastExt.empty()) {
                    extensions.insert(lastExt);
                }
            }

            // 收集文件
            std::vector<std::string> files;
#ifdef CCKIT_USE_GHC_FS
            fs::directory_iterator it(folder, ec);
            fs::directory_iterator end;
            while (it != end) {
                const auto& entry = *it;
                std::error_code ec2;
                if (entry.is_regular_file(ec2)) {
                    if (extensions.empty()) {
                        files.push_back(entry.path().generic_string());
                    }
                    else {
                        std::string ext = entry.path().extension().generic_string();
                        if (extensions.find(ext) != extensions.end()) {
                            files.push_back(entry.path().generic_string());
                        }
                    }
                }
                it.increment(ec);
            }
#else
            for (const auto& entry : fs::directory_iterator(folder)) {
                if (entry.is_regular_file()) {
                    if (extensions.empty()) {
                        files.push_back(entry.path().generic_string());
                    }
                    else {
                        std::string ext = entry.path().extension().generic_string();
                        if (extensions.find(ext) != extensions.end()) {
                            files.push_back(entry.path().generic_string());
                        }
                    }
                }
            }
#endif

            // 分配数组
            *outCount = files.size();
            if (files.empty()) {
                return nullptr;
            }

            char** array = (char**)malloc(files.size() * sizeof(char*));
            if (!array) {
                *outCount = 0;
                return nullptr;
            }

            // 复制字符串
            for (size_t i = 0; i < files.size(); ++i) {
                array[i] = strdup(files[i].c_str());
            }

            return array;
        }
        catch (...) {
            if (outCount) *outCount = 0;
            return nullptr;
        }
    }

    // ========================================
    // 内存管理
    // ========================================

    void cckit_fs_free_string(char* str) {
        if (str) {
            free(str);
        }
    }

    void cckit_fs_free_string_array(char** array, size_t count) {
        if (!array) return;
        for (size_t i = 0; i < count; ++i) {
            if (array[i]) {
                free(array[i]);
            }
        }
        free(array);
    }

} // extern "C"
