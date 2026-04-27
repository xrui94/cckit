// fs.hpp - 文件系统工具 C++ 包装层
#pragma once

#include "capi/fs.h"

#include <string>
#include <vector>
#include <optional>
#include <set>

namespace cckit::fs
{

    // ========================================
    // 路径查询
    // ========================================

    inline bool exists(const std::string& path) {
        return cckit_fs_exists(path.c_str());
    }

    inline bool isRegularFile(const std::string& path) {
        return cckit_fs_is_regular_file(path.c_str());
    }

    inline bool isDirectory(const std::string& path) {
        return cckit_fs_is_directory(path.c_str());
    }

    inline bool isSymlink(const std::string& path) {
        return cckit_fs_is_symlink(path.c_str());
    }

    inline std::string getSeparator() {
        return std::string(cckit_fs_get_separator());
    }

    inline std::string getGenericPath(const std::string& path) {
        char* result = cckit_fs_get_generic_path(path.c_str());
        std::string resultStr(result ? result : "");
        cckit_fs_free_string(result);
        return resultStr;
    }

    inline std::string getAbsolutePath(const std::string& path) {
        char* result = cckit_fs_get_absolute_path(path.c_str());
        std::string resultStr(result ? result : "");
        cckit_fs_free_string(result);
        return resultStr;
    }

    inline std::string getCurrentDirectory() {
        char* result = cckit_fs_get_current_directory();
        std::string resultStr(result ? result : "");
        cckit_fs_free_string(result);
        return resultStr;
    }

    inline std::string getParentPath(const std::string& path) {
        char* result = cckit_fs_get_parent_path(path.c_str());
        std::string resultStr(result ? result : "");
        cckit_fs_free_string(result);
        return resultStr;
    }

    inline std::string getFileName(const std::string& path) {
        char* result = cckit_fs_get_file_name(path.c_str());
        std::string resultStr(result ? result : "");
        cckit_fs_free_string(result);
        return resultStr;
    }

    inline std::string getFileBase(const std::string& path) {
        char* result = cckit_fs_get_file_base(path.c_str());
        std::string resultStr(result ? result : "");
        cckit_fs_free_string(result);
        return resultStr;
    }

    inline std::string getFileSuffix(const std::string& path) {
        char* result = cckit_fs_get_file_suffix(path.c_str());
        std::string resultStr(result ? result : "");
        cckit_fs_free_string(result);
        return resultStr;
    }

    inline std::string getDirectoryPath(const std::string& path) {
        char* result = cckit_fs_get_directory_path(path.c_str());
        std::string resultStr(result ? result : "");
        cckit_fs_free_string(result);
        return resultStr;
    }

    inline std::string getDirectoryName(const std::string& path) {
        char* result = cckit_fs_get_directory_name(path.c_str());
        std::string resultStr(result ? result : "");
        cckit_fs_free_string(result);
        return resultStr;
    }

    inline std::string resolveRelativePath(std::string_view baseFileAbsolute, std::string_view relativePath) {
        std::string baseFileAbsoluteStr = std::string(baseFileAbsolute);
        std::string relativePathStr = std::string(relativePath);
        char* result = cckit_fs_resolve_relative_path(baseFileAbsoluteStr.c_str(), relativePathStr.c_str());
        std::string resultStr(result ? result : "");
        cckit_fs_free_string(result);
        return resultStr;
    }

    // ========================================
    // 文件操作
    // ========================================

    inline bool copyFile(const std::string& srcFilename, const std::string& dstFilename, bool createNewPath = false) {
        return cckit_fs_copy_file(srcFilename.c_str(), dstFilename.c_str(), createNewPath);
    }

    inline bool removeFile(const std::string& path) {
        return cckit_fs_remove_file(path.c_str());
    }

    inline std::optional<size_t> getFileSize(const std::string& path) {
        size_t size = 0;
        if (cckit_fs_get_file_size(path.c_str(), &size)) {
            return size;
        }
        return std::nullopt;
    }

    // ========================================
    // 目录操作
    // ========================================

    inline bool ensurePathExists(const std::string& path) {
        return cckit_fs_ensure_path_exists(path.c_str());
    }

    inline bool createDirectory(const std::string& path) {
        return cckit_fs_create_directory(path.c_str());
    }

    inline bool createSymlink(const std::string& sourcePath, const std::string& targetPath) {
        return cckit_fs_create_symlink(sourcePath.c_str(), targetPath.c_str());
    }

    inline bool removeDirectory(const std::string& path) {
        return cckit_fs_remove_directory(path.c_str());
    }

    inline bool removeDirectoryAll(const std::string& path) {
        return cckit_fs_remove_directory_all(path.c_str());
    }

    std::vector<std::string> listFiles(const std::string& folder, const std::set<std::string>& matchExtensions = {});

} // namespace cckit::fs
