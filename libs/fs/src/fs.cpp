// fs.cpp - 文件系统工具实现
#include "cckit/fs/fs.h"
#include "cckit/export.h"

#include <filesystem>
#include <cstring>
#include <algorithm>
#include <set>

#ifdef _WIN32
    // Windows 平台使用 _strdup 代替 strdup
    #define strdup _strdup
#endif

namespace fs = std::filesystem;

extern "C" {

// ========================================
// 路径查询实现
// ========================================

bool cckit_fs_exists(const char* path) {
    if (!path) return false;
    try {
        return fs::exists(path);
    } catch (...) {
        return false;
    }
}

bool cckit_fs_is_regular_file(const char* path) {
    if (!path) return false;
    try {
        return fs::is_regular_file(path);
    } catch (...) {
        return false;
    }
}

bool cckit_fs_is_directory(const char* path) {
    if (!path) return false;
    try {
        return fs::is_directory(path);
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
        fs::path p = fs::absolute(path);
        std::string result = p.string();
        return strdup(result.c_str());
    } catch (...) {
        return nullptr;
    }
}

char* cckit_fs_get_current_directory(void) {
    try {
        fs::path p = fs::current_path();
        std::string result = p.string();
        return strdup(result.c_str());
    } catch (...) {
        return nullptr;
    }
}

char* cckit_fs_get_parent_path(const char* path) {
    if (!path) return nullptr;
    try {
        fs::path p(path);
        fs::path parent = p.parent_path();
        std::string result = parent.string();
        return strdup(result.c_str());
    } catch (...) {
        return nullptr;
    }
}

char* cckit_fs_get_file_name(const char* path) {
    if (!path) return nullptr;
    try {
        fs::path p(path);
        std::string result = p.filename().string();
        return strdup(result.c_str());
    } catch (...) {
        return nullptr;
    }
}

char* cckit_fs_get_file_base(const char* path) {
    if (!path) return nullptr;
    try {
        fs::path p(path);
        std::string result = p.stem().string();
        return strdup(result.c_str());
    } catch (...) {
        return nullptr;
    }
}

char* cckit_fs_get_file_suffix(const char* path) {
    if (!path) return nullptr;
    try {
        fs::path p(path);
        std::string result = p.extension().string();
        return strdup(result.c_str());
    } catch (...) {
        return nullptr;
    }
}

char* cckit_fs_get_directory_name(const char* path) {
    if (!path) return nullptr;
    try {
        fs::path p(path);
        std::string result = p.filename().string();
        return strdup(result.c_str());
    } catch (...) {
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
                fs::create_directories(dstPath.parent_path());
            }
        }
        fs::copy_file(srcFilename, dstFilename, fs::copy_options::overwrite_existing);
        return true;
    } catch (...) {
        return false;
    }
}

bool cckit_fs_remove_file(const char* path) {
    if (!path) return false;
    try {
        return fs::remove(path);
    } catch (...) {
        return false;
    }
}

bool cckit_fs_get_file_size(const char* path, size_t* outSize) {
    if (!path || !outSize) return false;
    try {
        *outSize = fs::file_size(path);
        return true;
    } catch (...) {
        return false;
    }
}

// ========================================
// 目录操作实现
// ========================================

bool cckit_fs_ensure_path_exists(const char* path) {
    if (!path) return false;
    try {
        if (fs::exists(path)) {
            return fs::is_directory(path);
        }
        return fs::create_directories(path);
    } catch (...) {
        return false;
    }
}

bool cckit_fs_create_directory(const char* path) {
    if (!path) return false;
    try {
        return fs::create_directories(path);
    } catch (...) {
        return false;
    }
}

bool cckit_fs_remove_directory(const char* path) {
    if (!path) return false;
    try {
        return fs::remove(path);
    } catch (...) {
        return false;
    }
}

bool cckit_fs_remove_directory_all(const char* path) {
    if (!path) return false;
    try {
        return fs::remove_all(path) > 0;
    } catch (...) {
        return false;
    }
}

char** cckit_fs_list_files(const char* folder, const char* matchExtensions, size_t* outCount) {
    if (!folder || !outCount) {
        if (outCount) *outCount = 0;
        return nullptr;
    }

    try {
        if (!fs::is_directory(folder)) {
            *outCount = 0;
            return nullptr;
        }

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
        for (const auto& entry : fs::directory_iterator(folder)) {
            if (entry.is_regular_file()) {
                if (extensions.empty()) {
                    files.push_back(entry.path().string());
                } else {
                    std::string ext = entry.path().extension().string();
                    if (extensions.find(ext) != extensions.end()) {
                        files.push_back(entry.path().string());
                    }
                }
            }
        }

        // 分配结果数组
        *outCount = files.size();
        char** result = static_cast<char**>(malloc((*outCount) * sizeof(char*)));
        if (!result) {
            *outCount = 0;
            return nullptr;
        }

        for (size_t i = 0; i < *outCount; ++i) {
            result[i] = strdup(files[i].c_str());
        }

        return result;
    } catch (...) {
        *outCount = 0;
        return nullptr;
    }
}

// ========================================
// 内存管理实现
// ========================================

void cckit_fs_free_string(char* str) {
    if (str) {
        free(str);
    }
}

void cckit_fs_free_string_array(char** array, size_t count) {
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
