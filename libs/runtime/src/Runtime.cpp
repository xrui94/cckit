// Runtime.cpp - 运行时工具实现
#include "cckit/runtime/Runtime.hpp"

#include <filesystem>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <cstdint>

#if defined(_WIN32) || defined(_WIN64)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <libloaderapi.h>
    #include <psapi.h>
    #include <lmcons.h>
    #include <shlobj.h>
    #include <userenv.h>
    #include <winternl.h>
    #pragma comment(lib, "userenv.Lib")
    #pragma comment(lib, "shell32.Lib")
    #pragma comment(lib, "psapi.Lib")
#elif defined(__linux__)
    #include <climits>
    #include <dlfcn.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/sysinfo.h>
    #include <pwd.h>
    #include <sys/utsname.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
    #include <climits>
    #include <dlfcn.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/sysctl.h>
    #include <pwd.h>
    #include <mach/mach.h>
    #include <mach/mach_host.h>
#endif

namespace cckit::runtime
{

    namespace fs = std::filesystem;

    // ========================================
    // C++ 便捷函数（非 inline，需要转换复杂类型）
    // ========================================

    std::vector<std::string> getArgv()
    {
        int argc = cckit_runtime_get_argc();
        char** argv = cckit_runtime_get_argv();
        
        std::vector<std::string> result;
        if (argc > 0 && argv != nullptr)
        {
            result.reserve(static_cast<size_t>(argc));
            for (int i = 0; i < argc; ++i)
            {
                if (argv[i] != nullptr)
                {
                    result.push_back(std::string(argv[i]));
                }
            }
        }
        return result;
    }

    std::map<std::string, std::string> getAllEnv()
    {
        std::map<std::string, std::string> envMap;

#if defined(_WIN32) || defined(_WIN64)
        // Windows: 使用 GetEnvironmentStringsW
        wchar_t* envStrings = GetEnvironmentStringsW();
        if (envStrings)
        {
            wchar_t* ptr = envStrings;
            while (*ptr != L'\0')
            {
                std::wstring envStr(ptr);
                size_t pos = envStr.find(L'=');
                if (pos != std::wstring::npos && pos > 0)  // 跳过以 '=' 开头的特殊变量
                {
                    std::wstring name = envStr.substr(0, pos);
                    std::wstring value = envStr.substr(pos + 1);

                    // 转换为 UTF-8
                    int nameSize = WideCharToMultiByte(CP_UTF8, 0, name.c_str(), static_cast<int>(name.length()),
                                                       nullptr, 0, nullptr, nullptr);
                    int valueSize = WideCharToMultiByte(CP_UTF8, 0, value.c_str(), static_cast<int>(value.length()),
                                                        nullptr, 0, nullptr, nullptr);

                    if (nameSize > 0)
                    {
                        std::string nameUtf8(nameSize, '\0');
                        WideCharToMultiByte(CP_UTF8, 0, name.c_str(), static_cast<int>(name.length()),
                                           &nameUtf8[0], nameSize, nullptr, nullptr);

                        std::string valueUtf8;
                        if (valueSize > 0)
                        {
                            valueUtf8.resize(valueSize);
                            WideCharToMultiByte(CP_UTF8, 0, value.c_str(), static_cast<int>(value.length()),
                                               &valueUtf8[0], valueSize, nullptr, nullptr);
                        }

                        envMap[nameUtf8] = valueUtf8;
                    }
                }
                ptr += envStr.length() + 1;
            }
            FreeEnvironmentStringsW(envStrings);
        }
#else
        // Linux/macOS: 使用 environ
        extern char** environ;
        if (environ)
        {
            for (int i = 0; environ[i] != nullptr; ++i)
            {
                std::string envStr(environ[i]);
                size_t pos = envStr.find('=');
                if (pos != std::string::npos)
                {
                    std::string name = envStr.substr(0, pos);
                    std::string value = envStr.substr(pos + 1);
                    envMap[name] = value;
                }
            }
        }
#endif

        return envMap;
    }

    std::vector<std::string> getPathEnv()
    {
        char* pathEnv = cckit_runtime_get_env("PATH");
        std::vector<std::string> paths;

        if (pathEnv != nullptr)
        {
            std::string pathEnvStr(pathEnv);
            cckit_runtime_free_string(pathEnv);
            
            std::stringstream ss(pathEnvStr);
            std::string item;

#if defined(_WIN32) || defined(_WIN64)
            // Windows: 使用分号分隔
            while (std::getline(ss, item, ';'))
            {
                if (!item.empty())
                {
                    paths.push_back(item);
                }
            }
#else
            // Linux/macOS: 使用冒号分隔
            while (std::getline(ss, item, ':'))
            {
                if (!item.empty())
                {
                    paths.push_back(item);
                }
            }
#endif
        }

        return paths;
    }

} // namespace cckit::runtime

// ========================================
// C API 实现
// ========================================

extern "C" {

// 辅助函数：将宽字符转换为 UTF-8 字符串（新分配）
static char* wstring_to_utf8_alloc(const wchar_t* wstr, int length)
{
    if (wstr == nullptr || length <= 0)
    {
        return nullptr;
    }

    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wstr, length,
                                       nullptr, 0, nullptr, nullptr);
    if (utf8Size > 0)
    {
        char* result = static_cast<char*>(malloc(static_cast<size_t>(utf8Size) + 1));
        if (result)
        {
            WideCharToMultiByte(CP_UTF8, 0, wstr, length,
                               result, utf8Size, nullptr, nullptr);
            result[utf8Size] = '\0';
            return result;
        }
    }
    return nullptr;
}

// 辅助函数：复制字符串（新分配）
static char* string_dup(const char* str)
{
    if (str == nullptr)
    {
        return nullptr;
    }
    size_t len = strlen(str);
    char* result = static_cast<char*>(malloc(len + 1));
    if (result)
    {
        memcpy(result, str, len + 1);
    }
    return result;
}

// ========================================
// 可执行文件路径
// ========================================

CCKIT_API char* cckit_runtime_get_executable_path(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 GetModuleFileNameW 获取可执行文件路径
    wchar_t wpath[MAX_PATH];
    DWORD length = GetModuleFileNameW(NULL, wpath, MAX_PATH);

    // 检查是否缓冲区不足
    if (length == 0 || length == MAX_PATH)
    {
        // 缓冲区不足，尝试动态分配
        DWORD size = MAX_PATH * 2;
        wchar_t* dynamic_wpath = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * size));
        if (dynamic_wpath == nullptr)
        {
            return nullptr;
        }
        length = GetModuleFileNameW(NULL, dynamic_wpath, size);

        char* result = nullptr;
        if (length > 0 && length < size)
        {
            result = wstring_to_utf8_alloc(dynamic_wpath, static_cast<int>(length));
        }
        free(dynamic_wpath);
        return result;
    }
    else
    {
        return wstring_to_utf8_alloc(wpath, static_cast<int>(length));
    }

#elif defined(__linux__)
    // Linux: 读取 /proc/self/exe 符号链接
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1)
    {
        buffer[len] = '\0';
        return string_dup(buffer);
    }
    return nullptr;

#elif defined(__APPLE__)
    // macOS: 使用 _NSGetExecutablePath
    char buffer[PATH_MAX];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) == 0)
    {
        // 解析符号链接
        char real_path[PATH_MAX];
        if (realpath(buffer, real_path) != nullptr)
        {
            return string_dup(real_path);
        }
        return string_dup(buffer);
    }
    else
    {
        // 缓冲区不足，动态分配
        char* dynamic_buffer = static_cast<char*>(malloc(size));
        if (dynamic_buffer == nullptr)
        {
            return nullptr;
        }
        if (_NSGetExecutablePath(dynamic_buffer, &size) == 0)
        {
            char real_path[PATH_MAX];
            char* result;
            if (realpath(dynamic_buffer, real_path) != nullptr)
            {
                result = string_dup(real_path);
            }
            else
            {
                result = string_dup(dynamic_buffer);
            }
            free(dynamic_buffer);
            return result;
        }
        free(dynamic_buffer);
    }
    return nullptr;
#else
    return nullptr;
#endif
}

CCKIT_API char* cckit_runtime_get_executable_dir(void)
{
    char* exe_path = cckit_runtime_get_executable_path();
    if (exe_path == nullptr)
    {
        return nullptr;
    }

    // 查找最后一个路径分隔符
    char* last_sep = nullptr;
    for (char* p = exe_path; *p != '\0'; ++p)
    {
#if defined(_WIN32) || defined(_WIN64)
        if (*p == '\\' || *p == '/')
#else
        if (*p == '/')
#endif
        {
            last_sep = p;
        }
    }

    if (last_sep != nullptr)
    {
        *last_sep = '\0';
        return exe_path;
    }

    cckit_runtime_free_string(exe_path);
    return nullptr;
}

// ========================================
// 动态库路径
// ========================================

CCKIT_API char* cckit_runtime_get_library_path(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 GetModuleHandleExW 获取当前 DLL 的模块句柄
    HMODULE hModule = NULL;
    if (!GetModuleHandleExW(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCWSTR>(&cckit_runtime_get_library_path),
        &hModule))
    {
        return nullptr;
    }

    // 获取 DLL 的完整宽字符路径
    wchar_t wpath[MAX_PATH];
    DWORD length = GetModuleFileNameW(hModule, wpath, MAX_PATH);

    // 检查是否缓冲区不足
    if (length == 0 || length == MAX_PATH)
    {
        // 缓冲区不足，尝试动态分配
        DWORD size = MAX_PATH * 2;
        wchar_t* dynamic_wpath = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * size));
        if (dynamic_wpath == nullptr)
        {
            return nullptr;
        }
        length = GetModuleFileNameW(hModule, dynamic_wpath, size);

        char* result = nullptr;
        if (length > 0 && length < size)
        {
            result = wstring_to_utf8_alloc(dynamic_wpath, static_cast<int>(length));
        }
        free(dynamic_wpath);
        return result;
    }
    else
    {
        return wstring_to_utf8_alloc(wpath, static_cast<int>(length));
    }

#else
    // Linux/macOS: 使用 dladdr 获取库信息
    Dl_info dlInfo;
    if (dladdr(reinterpret_cast<void*>(&cckit_runtime_get_library_path), &dlInfo) != 0)
    {
        if (dlInfo.dli_fname)
        {
            // 解析为绝对路径
            char real_path[PATH_MAX];
            if (realpath(dlInfo.dli_fname, real_path) != nullptr)
            {
                return string_dup(real_path);
            }
            return string_dup(dlInfo.dli_fname);
        }
    }
    return nullptr;
#endif
}

CCKIT_API char* cckit_runtime_get_library_dir(void)
{
    char* full_path = cckit_runtime_get_library_path();
    if (full_path == nullptr)
    {
        return nullptr;
    }

    // 查找最后一个路径分隔符
    char* last_sep = nullptr;
    for (char* p = full_path; *p != '\0'; ++p)
    {
#if defined(_WIN32) || defined(_WIN64)
        if (*p == '\\' || *p == '/')
#else
        if (*p == '/')
#endif
        {
            last_sep = p;
        }
    }

    if (last_sep != nullptr)
    {
        *last_sep = '\0';
        return full_path;
    }

    cckit_runtime_free_string(full_path);
    return nullptr;
}

// ========================================
// 进程信息
// ========================================

CCKIT_API int cckit_runtime_get_process_id(void)
{
#if defined(_WIN32) || defined(_WIN64)
    return static_cast<int>(GetCurrentProcessId());
#else
    return static_cast<int>(getpid());
#endif
}

CCKIT_API int cckit_runtime_get_parent_process_id(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 NtQueryInformationProcess 获取父进程 ID
    // 定义 PROCESS_BASIC_INFORMATION 结构体（因为不同 SDK 版本可能不同）
    typedef struct _PROCESS_BASIC_INFORMATION {
        NTSTATUS ExitStatus;
        PVOID PebBaseAddress;
        ULONG_PTR AffinityMask;
        LONG BasePriority;
        ULONG_PTR UniqueProcessId;
        ULONG_PTR InheritedFromUniqueProcessId;
    } PROCESS_BASIC_INFORMATION;

    typedef NTSTATUS (WINAPI *PFN_NtQueryInformationProcess)(
        HANDLE, LONG, PVOID, ULONG, PULONG);

    DWORD ppid = 0;
    HMODULE hNtDll = GetModuleHandleW(L"ntdll.dll");
    if (hNtDll)
    {
        PFN_NtQueryInformationProcess pfnNtQueryInformationProcess =
            reinterpret_cast<PFN_NtQueryInformationProcess>(
                GetProcAddress(hNtDll, "NtQueryInformationProcess"));

        if (pfnNtQueryInformationProcess)
        {
            PROCESS_BASIC_INFORMATION pbi;
            memset(&pbi, 0, sizeof(pbi));
            ULONG returnLength = 0;
            if (pfnNtQueryInformationProcess(GetCurrentProcess(), 0,
                                             &pbi, sizeof(pbi), &returnLength) == 0)
            {
                ppid = static_cast<DWORD>(pbi.InheritedFromUniqueProcessId);
            }
        }
    }

    return static_cast<int>(ppid);
#elif defined(__linux__)
    return static_cast<int>(getppid());
#elif defined(__APPLE__)
    struct kinfo_proc info;
    size_t length = sizeof(info);
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid()};

    if (sysctl(mib, 4, &info, &length, NULL, 0) == 0)
    {
        return static_cast<int>(info.kp_eproc.e_ppid);
    }
    return -1;
#else
    return -1;
#endif
}

CCKIT_API char* cckit_runtime_get_process_name(void)
{
    char* exe_path = cckit_runtime_get_executable_path();
    if (exe_path == nullptr)
    {
        return nullptr;
    }

    // 查找最后一个路径分隔符
    char* last_sep = nullptr;
    for (char* p = exe_path; *p != '\0'; ++p)
    {
#if defined(_WIN32) || defined(_WIN64)
        if (*p == '\\' || *p == '/')
#else
        if (*p == '/')
#endif
        {
            last_sep = p;
        }
    }

    char* result;
    if (last_sep != nullptr)
    {
        result = string_dup(last_sep + 1);
        cckit_runtime_free_string(exe_path);
    }
    else
    {
        result = exe_path;
    }
    return result;
}

CCKIT_API int cckit_runtime_get_argc(void)
{
    // 在 C API 中，main 函数的参数通常不可直接访问
    // 返回 0，实际使用时需要用户自己保存 argc
    return 0;
}

CCKIT_API char** cckit_runtime_get_argv(void)
{
    // 在 C API 中，main 函数的参数通常不可直接访问
    // 返回 nullptr，实际使用时需要用户自己保存 argv
    return nullptr;
}

CCKIT_API char* cckit_runtime_get_command_line(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 GetCommandLineW
    wchar_t* wcmd = GetCommandLineW();
    if (wcmd)
    {
        int length = static_cast<int>(wcslen(wcmd));
        return wstring_to_utf8_alloc(wcmd, length);
    }
    return nullptr;
#else
    // Linux/macOS: 从 /proc/self/cmdline 读取
    std::ifstream file("/proc/self/cmdline");
    if (file.is_open())
    {
        std::string cmdline;
        std::string arg;
        while (std::getline(file, arg, '\0'))
        {
            if (!cmdline.empty())
            {
                cmdline += " ";
            }
            cmdline += arg;
        }
        file.close();
        return string_dup(cmdline.c_str());
    }
    return nullptr;
#endif
}

// ========================================
// 环境变量
// ========================================

CCKIT_API char* cckit_runtime_get_env(const char* name)
{
    if (name == nullptr)
    {
        return nullptr;
    }

#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 _wgetenv_s
    wchar_t wname[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, name, -1, wname, MAX_PATH);

    size_t requiredSize = 0;
    _wgetenv_s(&requiredSize, nullptr, 0, wname);

    if (requiredSize > 0)
    {
        wchar_t* wvalue = static_cast<wchar_t*>(malloc(sizeof(wchar_t) * requiredSize));
        if (wvalue == nullptr)
        {
            return nullptr;
        }
        if (_wgetenv_s(&requiredSize, wvalue, requiredSize, wname) == 0)
        {
            char* result = wstring_to_utf8_alloc(wvalue, static_cast<int>(requiredSize - 1));
            free(wvalue);
            return result;
        }
        free(wvalue);
    }
    return nullptr;
#else
    // Linux/macOS: 使用 getenv
    char* value = std::getenv(name);
    return value ? string_dup(value) : nullptr;
#endif
}

CCKIT_API char* cckit_runtime_get_env_or_default(const char* name, const char* default_value)
{
    char* value = cckit_runtime_get_env(name);
    if (value != nullptr)
    {
        return value;
    }
    return default_value ? string_dup(default_value) : nullptr;
}

CCKIT_API bool cckit_runtime_set_env(const char* name, const char* value)
{
    if (name == nullptr || value == nullptr)
    {
        return false;
    }

#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 _wputenv_s
    wchar_t wname[MAX_PATH];
    wchar_t wvalue[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, name, -1, wname, MAX_PATH);
    MultiByteToWideChar(CP_UTF8, 0, value, -1, wvalue, MAX_PATH);
    return _wputenv_s(wname, wvalue) == 0;
#else
    // Linux/macOS: 使用 setenv
    return setenv(name, value, 1) == 0;
#endif
}

CCKIT_API bool cckit_runtime_unset_env(const char* name)
{
    if (name == nullptr)
    {
        return false;
    }

#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 _wputenv_s 设置为空
    wchar_t wname[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, name, -1, wname, MAX_PATH);
    return _wputenv_s(wname, L"") == 0;
#else
    // Linux/macOS: 使用 unsetenv
    return unsetenv(name) == 0;
#endif
}

CCKIT_API char** cckit_runtime_get_path_env(size_t* out_count)
{
    if (out_count == nullptr)
    {
        return nullptr;
    }

    char* pathEnv = cckit_runtime_get_env("PATH");
    if (pathEnv == nullptr)
    {
        *out_count = 0;
        return nullptr;
    }

    // 计算路径数量
    size_t count = 0;
    const char* p = pathEnv;
    while (*p != '\0')
    {
#if defined(_WIN32) || defined(_WIN64)
        if (*p == ';')
#else
        if (*p == ':')
#endif
        {
            ++count;
        }
        ++p;
    }
    count += 1; // 最后一个路径

    // 分配数组
    char** result = static_cast<char**>(malloc(sizeof(char*) * (count + 1)));
    if (result == nullptr)
    {
        cckit_runtime_free_string(pathEnv);
        *out_count = 0;
        return nullptr;
    }

    // 解析路径
    size_t index = 0;
    char* pathStart = pathEnv;
    char* pathEnd = pathEnv;
    while (*pathEnd != '\0')
    {
#if defined(_WIN32) || defined(_WIN64)
        if (*pathEnd == ';')
#else
        if (*pathEnd == ':')
#endif
        {
            *pathEnd = '\0';
            if (pathStart != pathEnd)
            {
                result[index] = string_dup(pathStart);
                if (result[index] != nullptr)
                {
                    ++index;
                }
            }
            pathStart = pathEnd + 1;
        }
        ++pathEnd;
    }
    // 最后一个路径
    if (*pathStart != '\0')
    {
        result[index] = string_dup(pathStart);
        if (result[index] != nullptr)
        {
            ++index;
        }
    }
    result[index] = nullptr;

    cckit_runtime_free_string(pathEnv);
    *out_count = index;
    return result;
}

// ========================================
// 工作目录
// ========================================

CCKIT_API char* cckit_runtime_get_current_working_directory(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 GetCurrentDirectory
    wchar_t wpath[MAX_PATH];
    DWORD length = GetCurrentDirectoryW(MAX_PATH, wpath);
    if (length > 0 && length < MAX_PATH)
    {
        return wstring_to_utf8_alloc(wpath, static_cast<int>(length));
    }
    return nullptr;
#else
    // Linux/macOS: 使用 getcwd
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != nullptr)
    {
        return string_dup(buffer);
    }
    return nullptr;
#endif
}

CCKIT_API bool cckit_runtime_set_current_working_directory(const char* path)
{
    if (path == nullptr)
    {
        return false;
    }

#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 SetCurrentDirectory
    wchar_t wpath[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, MAX_PATH);
    return SetCurrentDirectoryW(wpath) != 0;
#else
    // Linux/macOS: 使用 chdir
    return chdir(path) == 0;
#endif
}

CCKIT_API char* cckit_runtime_get_temp_directory(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 GetTempPath
    wchar_t wpath[MAX_PATH];
    DWORD length = GetTempPathW(MAX_PATH, wpath);
    if (length > 0 && length < MAX_PATH)
    {
        // 移除末尾的反斜杠
        if (length > 1 && (wpath[length - 1] == L'\\' || wpath[length - 1] == L'/'))
        {
            wpath[length - 1] = L'\0';
            length--;
        }
        return wstring_to_utf8_alloc(wpath, static_cast<int>(length));
    }
    return nullptr;
#else
    // Linux/macOS: 使用 TMPDIR 或 /tmp
    const char* tmpdir = std::getenv("TMPDIR");
    if (tmpdir)
    {
        // 移除末尾的斜杠
        size_t len = strlen(tmpdir);
        if (len > 1 && (tmpdir[len - 1] == '/' || tmpdir[len - 1] == '\\'))
        {
            char* result = static_cast<char*>(malloc(len));
            if (result)
            {
                memcpy(result, tmpdir, len - 1);
                result[len - 1] = '\0';
                return result;
            }
            return nullptr;
        }
        return string_dup(tmpdir);
    }
    return string_dup("/tmp");
#endif
}

CCKIT_API char* cckit_runtime_create_temp_directory(const char* prefix)
{
    const char* actualPrefix = (prefix != nullptr && prefix[0] != '\0') ? prefix : "cckit_";
    
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 GetTempFileName 创建临时文件，然后删除它并创建目录
    char* tempDir = cckit_runtime_get_temp_directory();
    if (tempDir == nullptr)
    {
        return nullptr;
    }
    
    // 转换为宽字符
    wchar_t wtempDir[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, tempDir, -1, wtempDir, MAX_PATH);
    
    wchar_t wprefix[64];
    MultiByteToWideChar(CP_UTF8, 0, actualPrefix, -1, wprefix, 64);
    
    wchar_t wtempPath[MAX_PATH];
    if (GetTempFileNameW(wtempDir, wprefix, 0, wtempPath) != 0)
    {
        // 删除文件，创建目录
        DeleteFileW(wtempPath);
        if (CreateDirectoryW(wtempPath, nullptr) != 0)
        {
            cckit_runtime_free_string(tempDir);
            return wstring_to_utf8_alloc(wtempPath, static_cast<int>(wcslen(wtempPath)));
        }
    }
    cckit_runtime_free_string(tempDir);
    return nullptr;
#else
    // Linux/macOS: 使用 mkdtemp
    char* tempDir = cckit_runtime_get_temp_directory();
    if (tempDir == nullptr)
    {
        return nullptr;
    }
    
    size_t tempDirLen = strlen(tempDir);
    size_t prefixLen = strlen(actualPrefix);
    // 路径格式: tempDir/prefixXXXXXX
    size_t totalLen = tempDirLen + 1 + prefixLen + 6 + 1;  // +1 for '/', +6 for XXXXXX, +1 for '\0'
    char* tempPath = static_cast<char*>(malloc(totalLen));
    if (tempPath == nullptr)
    {
        cckit_runtime_free_string(tempDir);
        return nullptr;
    }
    
    snprintf(tempPath, totalLen, "%s/%sXXXXXX", tempDir, actualPrefix);
    cckit_runtime_free_string(tempDir);
    
    if (mkdtemp(tempPath) != nullptr)
    {
        return tempPath;
    }
    free(tempPath);
    return nullptr;
#endif
}

CCKIT_API char* cckit_runtime_create_temp_file(const char* prefix)
{
    const char* actualPrefix = (prefix != nullptr && prefix[0] != '\0') ? prefix : "cckit_";
    
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 GetTempFileName
    char* tempDir = cckit_runtime_get_temp_directory();
    if (tempDir == nullptr)
    {
        return nullptr;
    }
    
    // 转换为宽字符
    wchar_t wtempDir[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, tempDir, -1, wtempDir, MAX_PATH);
    
    wchar_t wprefix[64];
    MultiByteToWideChar(CP_UTF8, 0, actualPrefix, -1, wprefix, 64);
    
    wchar_t wtempPath[MAX_PATH];
    if (GetTempFileNameW(wtempDir, wprefix, 0, wtempPath) != 0)
    {
        cckit_runtime_free_string(tempDir);
        return wstring_to_utf8_alloc(wtempPath, static_cast<int>(wcslen(wtempPath)));
    }
    cckit_runtime_free_string(tempDir);
    return nullptr;
#else
    // Linux/macOS: 使用 mkstemp
    char* tempDir = cckit_runtime_get_temp_directory();
    if (tempDir == nullptr)
    {
        return nullptr;
    }
    
    size_t tempDirLen = strlen(tempDir);
    size_t prefixLen = strlen(actualPrefix);
    // 路径格式: tempDir/prefixXXXXXX
    size_t totalLen = tempDirLen + 1 + prefixLen + 6 + 1;  // +1 for '/', +6 for XXXXXX, +1 for '\0'
    char* tempPath = static_cast<char*>(malloc(totalLen));
    if (tempPath == nullptr)
    {
        cckit_runtime_free_string(tempDir);
        return nullptr;
    }
    
    snprintf(tempPath, totalLen, "%s/%sXXXXXX", tempDir, actualPrefix);
    cckit_runtime_free_string(tempDir);
    
    int fd = mkstemp(tempPath);
    if (fd != -1)
    {
        close(fd);
        return tempPath;
    }
    free(tempPath);
    return nullptr;
#endif
}

// ========================================
// 标准路径
// ========================================

CCKIT_API char* cckit_runtime_get_home_directory(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 SHGetFolderPathW
    wchar_t wpath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_PROFILE, nullptr, 0, wpath)))
    {
        return wstring_to_utf8_alloc(wpath, static_cast<int>(wcslen(wpath)));
    }
    return nullptr;
#else
    // Linux/macOS: 使用 getpwuid
    struct passwd* pw = getpwuid(getuid());
    if (pw)
    {
        return string_dup(pw->pw_dir);
    }
    return nullptr;
#endif
}

CCKIT_API char* cckit_runtime_get_user_config_directory(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 SHGetFolderPathW (CSIDL_APPDATA)
    wchar_t wpath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_APPDATA, nullptr, 0, wpath)))
    {
        return wstring_to_utf8_alloc(wpath, static_cast<int>(wcslen(wpath)));
    }
    return nullptr;
#elif defined(__APPLE__)
    // macOS: ~/Library/Application Support
    char* home = cckit_runtime_get_home_directory();
    if (home != nullptr)
    {
        size_t len = strlen(home);
        char* result = static_cast<char*>(malloc(len + 28)); // "/Library/Application Support" = 27 chars
        if (result)
        {
            memcpy(result, home, len);
            memcpy(result + len, "/Library/Application Support", 28);
            cckit_runtime_free_string(home);
            return result;
        }
        cckit_runtime_free_string(home);
    }
    return nullptr;
#else
    // Linux: ~/.config
    char* configDir = cckit_runtime_get_env("XDG_CONFIG_HOME");
    if (configDir != nullptr)
    {
        return configDir;
    }
    char* home = cckit_runtime_get_home_directory();
    if (home != nullptr)
    {
        size_t len = strlen(home);
        char* result = static_cast<char*>(malloc(len + 8)); // "/.config" = 7 chars
        if (result)
        {
            memcpy(result, home, len);
            memcpy(result + len, "/.config", 8);
            cckit_runtime_free_string(home);
            return result;
        }
        cckit_runtime_free_string(home);
    }
    return nullptr;
#endif
}

CCKIT_API char* cckit_runtime_get_user_data_directory(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 SHGetFolderPathW (CSIDL_LOCAL_APPDATA)
    wchar_t wpath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, wpath)))
    {
        return wstring_to_utf8_alloc(wpath, static_cast<int>(wcslen(wpath)));
    }
    return nullptr;
#elif defined(__APPLE__)
    // macOS: ~/Library/Application Support
    return cckit_runtime_get_user_config_directory();
#else
    // Linux: ~/.local/share
    char* dataDir = cckit_runtime_get_env("XDG_DATA_HOME");
    if (dataDir != nullptr)
    {
        return dataDir;
    }
    char* home = cckit_runtime_get_home_directory();
    if (home != nullptr)
    {
        size_t len = strlen(home);
        char* result = static_cast<char*>(malloc(len + 12)); // "/.local/share" = 11 chars
        if (result)
        {
            memcpy(result, home, len);
            memcpy(result + len, "/.local/share", 12);
            cckit_runtime_free_string(home);
            return result;
        }
        cckit_runtime_free_string(home);
    }
    return nullptr;
#endif
}

CCKIT_API char* cckit_runtime_get_user_cache_directory(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 GetTempPath
    return cckit_runtime_get_temp_directory();
#elif defined(__APPLE__)
    // macOS: ~/Library/Caches
    char* home = cckit_runtime_get_home_directory();
    if (home != nullptr)
    {
        size_t len = strlen(home);
        char* result = static_cast<char*>(malloc(len + 16)); // "/Library/Caches" = 15 chars
        if (result)
        {
            memcpy(result, home, len);
            memcpy(result + len, "/Library/Caches", 16);
            cckit_runtime_free_string(home);
            return result;
        }
        cckit_runtime_free_string(home);
    }
    return nullptr;
#else
    // Linux: ~/.cache
    char* cacheDir = cckit_runtime_get_env("XDG_CACHE_HOME");
    if (cacheDir != nullptr)
    {
        return cacheDir;
    }
    char* home = cckit_runtime_get_home_directory();
    if (home != nullptr)
    {
        size_t len = strlen(home);
        char* result = static_cast<char*>(malloc(len + 7)); // "/.cache" = 6 chars
        if (result)
        {
            memcpy(result, home, len);
            memcpy(result + len, "/.cache", 7);
            cckit_runtime_free_string(home);
            return result;
        }
        cckit_runtime_free_string(home);
    }
    return nullptr;
#endif
}

CCKIT_API char* cckit_runtime_get_app_config_directory(const char* app_name)
{
    if (app_name == nullptr)
    {
        return nullptr;
    }
    char* configDir = cckit_runtime_get_user_config_directory();
    if (configDir == nullptr)
    {
        return nullptr;
    }
    size_t configLen = strlen(configDir);
    size_t appLen = strlen(app_name);
    char* result = static_cast<char*>(malloc(configLen + 1 + appLen + 1));
    if (result)
    {
        memcpy(result, configDir, configLen);
        result[configLen] = '/';
        memcpy(result + configLen + 1, app_name, appLen + 1);
    }
    cckit_runtime_free_string(configDir);
    return result;
}

CCKIT_API char* cckit_runtime_get_app_data_directory(const char* app_name)
{
    if (app_name == nullptr)
    {
        return nullptr;
    }
    char* dataDir = cckit_runtime_get_user_data_directory();
    if (dataDir == nullptr)
    {
        return nullptr;
    }
    size_t dataLen = strlen(dataDir);
    size_t appLen = strlen(app_name);
    char* result = static_cast<char*>(malloc(dataLen + 1 + appLen + 1));
    if (result)
    {
        memcpy(result, dataDir, dataLen);
        result[dataLen] = '/';
        memcpy(result + dataLen + 1, app_name, appLen + 1);
    }
    cckit_runtime_free_string(dataDir);
    return result;
}

CCKIT_API char* cckit_runtime_get_app_cache_directory(const char* app_name)
{
    if (app_name == nullptr)
    {
        return nullptr;
    }
    char* cacheDir = cckit_runtime_get_user_cache_directory();
    if (cacheDir == nullptr)
    {
        return nullptr;
    }
    size_t cacheLen = strlen(cacheDir);
    size_t appLen = strlen(app_name);
    char* result = static_cast<char*>(malloc(cacheLen + 1 + appLen + 1));
    if (result)
    {
        memcpy(result, cacheDir, cacheLen);
        result[cacheLen] = '/';
        memcpy(result + cacheLen + 1, app_name, appLen + 1);
    }
    cckit_runtime_free_string(cacheDir);
    return result;
}

// ========================================
// 系统信息
// ========================================

CCKIT_API char* cckit_runtime_get_os_name(void)
{
#if defined(_WIN32) || defined(_WIN64)
    return string_dup("Windows");
#elif defined(__linux__)
    return string_dup("Linux");
#elif defined(__APPLE__)
    return string_dup("macOS");
#else
    return string_dup("Unknown");
#endif
}

CCKIT_API char* cckit_runtime_get_os_version(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 RtlGetVersion
    typedef NTSTATUS (WINAPI *PFN_RtlGetVersion)(PRTL_OSVERSIONINFOW);
    HMODULE hNtDll = GetModuleHandleW(L"ntdll.dll");
    if (hNtDll)
    {
        PFN_RtlGetVersion pfnRtlGetVersion =
            reinterpret_cast<PFN_RtlGetVersion>(GetProcAddress(hNtDll, "RtlGetVersion"));
        if (pfnRtlGetVersion)
        {
            RTL_OSVERSIONINFOW rovi;
            ZeroMemory(&rovi, sizeof(RTL_OSVERSIONINFOW));
            rovi.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
            if (pfnRtlGetVersion(&rovi) == 0)
            {
                char buffer[64];
                snprintf(buffer, sizeof(buffer), "%lu.%lu.%lu",
                         rovi.dwMajorVersion, rovi.dwMinorVersion, rovi.dwBuildNumber);
                return string_dup(buffer);
            }
        }
    }
    return nullptr;
#elif defined(__linux__)
    // Linux: 读取 /etc/os-release
    std::ifstream file("/etc/os-release");
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (line.find("VERSION=") == 0)
            {
                size_t pos = line.find('=');
                if (pos != std::string::npos)
                {
                    std::string version = line.substr(pos + 1);
                    // 移除引号
                    if (version.size() >= 2 && version.front() == '"' && version.back() == '"')
                    {
                        version = version.substr(1, version.length() - 2);
                    }
                    return string_dup(version.c_str());
                }
            }
        }
        file.close();
    }
    return nullptr;
#elif defined(__APPLE__)
    // macOS: 使用 sw_vers 命令
    FILE* pipe = popen("sw_vers -productVersion", "r");
    if (pipe)
    {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), pipe) != nullptr)
        {
            // 移除换行符
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n')
            {
                buffer[len - 1] = '\0';
            }
            pclose(pipe);
            return string_dup(buffer);
        }
        pclose(pipe);
    }
    return nullptr;
#else
    return nullptr;
#endif
}

CCKIT_API char* cckit_runtime_get_os_architecture(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 GetNativeSystemInfo
    SYSTEM_INFO si;
    GetNativeSystemInfo(&si);

    switch (si.wProcessorArchitecture)
    {
        case PROCESSOR_ARCHITECTURE_AMD64:
            return string_dup("x86_64");
        case PROCESSOR_ARCHITECTURE_INTEL:
            return string_dup("x86");
        case PROCESSOR_ARCHITECTURE_ARM64:
            return string_dup("arm64");
        case PROCESSOR_ARCHITECTURE_ARM:
            return string_dup("arm");
        default:
            return string_dup("Unknown");
    }
#elif defined(__linux__)
    // Linux: 使用 uname
    struct utsname uts;
    if (uname(&uts) == 0)
    {
        return string_dup(uts.machine);
    }
    return string_dup("Unknown");
#elif defined(__APPLE__)
    // macOS: 使用 sysctl
    char buffer[128];
    size_t length = sizeof(buffer);
    if (sysctlbyname("hw.machine", buffer, &length, nullptr, 0) == 0)
    {
        return string_dup(buffer);
    }
    return string_dup("Unknown");
#else
    return string_dup("Unknown");
#endif
}

CCKIT_API char* cckit_runtime_get_host_name(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 GetComputerNameW
    wchar_t wname[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
    if (GetComputerNameW(wname, &size))
    {
        return wstring_to_utf8_alloc(wname, static_cast<int>(size));
    }
    return nullptr;
#else
    // Linux/macOS: 使用 gethostname
    char buffer[HOST_NAME_MAX];
    if (gethostname(buffer, sizeof(buffer)) == 0)
    {
        return string_dup(buffer);
    }
    return nullptr;
#endif
}

CCKIT_API char* cckit_runtime_get_user_name(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 GetUserNameW
    wchar_t wname[UNLEN + 1];
    DWORD size = UNLEN + 1;
    if (GetUserNameW(wname, &size))
    {
        return wstring_to_utf8_alloc(wname, static_cast<int>(size));
    }
    return nullptr;
#else
    // Linux/macOS: 使用 getpwuid
    struct passwd* pw = getpwuid(getuid());
    if (pw)
    {
        return string_dup(pw->pw_name);
    }
    return nullptr;
#endif
}

CCKIT_API int cckit_runtime_get_processor_count(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 GetSystemInfo
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return static_cast<int>(si.dwNumberOfProcessors);
#else
    // Linux/macOS: 使用 sysconf
    return static_cast<int>(sysconf(_SC_NPROCESSORS_ONLN));
#endif
}

CCKIT_API size_t cckit_runtime_get_total_memory(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 GlobalMemoryStatusEx
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    if (GlobalMemoryStatusEx(&status))
    {
        return static_cast<size_t>(status.ullTotalPhys);
    }
    return 0;
#elif defined(__linux__)
    // Linux: 使用 sysinfo
    struct sysinfo info;
    if (sysinfo(&info) == 0)
    {
        return static_cast<size_t>(info.totalram) * info.mem_unit;
    }
    return 0;
#elif defined(__APPLE__)
    // macOS: 使用 sysctl
    int64_t memory;
    size_t length = sizeof(memory);
    if (sysctlbyname("hw.memsize", &memory, &length, nullptr, 0) == 0)
    {
        return static_cast<size_t>(memory);
    }
    return 0;
#else
    return 0;
#endif
}

CCKIT_API size_t cckit_runtime_get_available_memory(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 GlobalMemoryStatusEx
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    if (GlobalMemoryStatusEx(&status))
    {
        return static_cast<size_t>(status.ullAvailPhys);
    }
    return 0;
#elif defined(__linux__)
    // Linux: 使用 sysinfo
    struct sysinfo info;
    if (sysinfo(&info) == 0)
    {
        return static_cast<size_t>(info.freeram) * info.mem_unit;
    }
    return 0;
#elif defined(__APPLE__)
    // macOS: 使用 mach host statistics
    vm_size_t page_size;
    vm_statistics64_data_t vm_stats;
    mach_msg_type_number_t count = sizeof(vm_stats) / sizeof(natural_t);

    host_page_size(mach_host_self(), &page_size);

    if (host_statistics64(mach_host_self(), HOST_VM_INFO,
                         (host_info64_t)&vm_stats, &count) == KERN_SUCCESS)
    {
        return static_cast<size_t>(vm_stats.free_count + vm_stats.inactive_count) * page_size;
    }
    return 0;
#else
    return 0;
#endif
}

// ========================================
// 时间信息
// ========================================

CCKIT_API uint64_t cckit_runtime_get_process_uptime(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 GetProcessTimes 获取进程运行时间
    FILETIME creationTime, exitTime, kernelTime, userTime;
    if (GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime, &kernelTime, &userTime))
    {
        // 获取当前时间
        FILETIME currentTime;
        GetSystemTimeAsFileTime(&currentTime);
        
        // 计算运行时间（100纳秒为单位）
        ULARGE_INTEGER creation, current;
        creation.LowPart = creationTime.dwLowDateTime;
        creation.HighPart = creationTime.dwHighDateTime;
        current.LowPart = currentTime.dwLowDateTime;
        current.HighPart = currentTime.dwHighDateTime;
        
        // 转换为毫秒
        return (current.QuadPart - creation.QuadPart) / 10000;
    }
    return 0;
#else
    // Linux/macOS: 使用 clock_gettime (CLOCK_MONOTONIC)
    // 注意：这返回的是系统启动后的时间，不是进程运行时间
    // 对于进程运行时间，应该使用 /proc/self/stat (Linux) 或其他方法
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
    {
        return static_cast<uint64_t>(ts.tv_sec) * 1000 +
               static_cast<uint64_t>(ts.tv_nsec) / 1000000;
    }
    return 0;
#endif
}

CCKIT_API uint64_t cckit_runtime_get_system_time(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 GetSystemTimeAsFileTime
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;

    // 转换为毫秒（从 1601-01-01 到 1970-01-01 的偏移量）
    return (uli.QuadPart - 116444736000000000ULL) / 10000;
#else
    // Linux/macOS: 使用 gettimeofday
    struct timeval tv;
    if (gettimeofday(&tv, nullptr) == 0)
    {
        return static_cast<uint64_t>(tv.tv_sec) * 1000 +
               static_cast<uint64_t>(tv.tv_usec) / 1000;
    }
    return 0;
#endif
}

CCKIT_API uint64_t cckit_runtime_get_monotonic_time(void)
{
#if defined(_WIN32) || defined(_WIN64)
    // Windows: 使用 QueryPerformanceCounter
    LARGE_INTEGER frequency, counter;
    if (QueryPerformanceFrequency(&frequency) && QueryPerformanceCounter(&counter))
    {
        return static_cast<uint64_t>(counter.QuadPart * 1000 / frequency.QuadPart);
    }
    return 0;
#else
    // Linux/macOS: 使用 clock_gettime (CLOCK_MONOTONIC)
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
    {
        return static_cast<uint64_t>(ts.tv_sec) * 1000 +
               static_cast<uint64_t>(ts.tv_nsec) / 1000000;
    }
    return 0;
#endif
}

// ========================================
// 内存管理
// ========================================

CCKIT_API void cckit_runtime_free_string(char* str)
{
    if (str != nullptr)
    {
        free(str);
    }
}

CCKIT_API void cckit_runtime_free_string_array(char** array, size_t count)
{
    if (array != nullptr)
    {
        for (size_t i = 0; i < count; ++i)
        {
            if (array[i] != nullptr)
            {
                free(array[i]);
            }
        }
        free(array);
    }
}

} // extern "C"
