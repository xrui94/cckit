// test_system.cpp - 系统信息测试
#include <gtest/gtest.h>
#include "cckit/runtime/Runtime.hpp"
#include <algorithm>

using namespace cckit::runtime;

class SystemTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // 测试前的设置
    }

    void TearDown() override
    {
        // 测试后的清理
    }
};

// ========================================
// 操作系统名称测试
// ========================================

TEST_F(SystemTest, GetOSName)
{
    std::string osName = getOSName();
    EXPECT_FALSE(osName.empty()) << "操作系统名称不应为空";

    // 验证返回值在预期范围内
    bool isValid = (osName == "Windows" || osName == "Linux" || osName == "macOS" || osName == "Unknown");
    EXPECT_TRUE(isValid) << "操作系统名称应为 Windows、Linux、macOS 或 Unknown";
}

TEST_F(SystemTest, GetOSNameConsistency)
{
    std::string osName1 = getOSName();
    std::string osName2 = getOSName();
    EXPECT_EQ(osName1, osName2) << "操作系统名称应保持一致";
}

// ========================================
// 操作系统版本测试
// ========================================

TEST_F(SystemTest, GetOSVersion)
{
    std::string osVersion = getOSVersion();
    // 注意：某些系统可能无法获取版本信息，所以不强制要求非空

    if (!osVersion.empty())
    {
        // 验证版本格式（通常包含数字）
        bool hasDigit = std::any_of(osVersion.begin(), osVersion.end(), ::isdigit);
        EXPECT_TRUE(hasDigit) << "操作系统版本应包含数字";
    }
}

TEST_F(SystemTest, GetOSVersionConsistency)
{
    std::string osVersion1 = getOSVersion();
    std::string osVersion2 = getOSVersion();
    EXPECT_EQ(osVersion1, osVersion2) << "操作系统版本应保持一致";
}

// ========================================
// 系统架构测试
// ========================================

TEST_F(SystemTest, GetOSArchitecture)
{
    std::string arch = getOSArchitecture();
    EXPECT_FALSE(arch.empty()) << "系统架构不应为空";

    // 验证返回值在预期范围内
    bool isValid = (arch == "x86_64" || arch == "x86" || arch == "arm64" || arch == "arm" || arch == "Unknown");
    EXPECT_TRUE(isValid) << "系统架构应为 x86_64、x86、arm64、arm 或 Unknown";
}

TEST_F(SystemTest, GetOSArchitectureConsistency)
{
    std::string arch1 = getOSArchitecture();
    std::string arch2 = getOSArchitecture();
    EXPECT_EQ(arch1, arch2) << "系统架构应保持一致";
}

// ========================================
// 主机名测试
// ========================================

TEST_F(SystemTest, GetHostName)
{
    std::string hostName = getHostName();
    EXPECT_FALSE(hostName.empty()) << "主机名不应为空";

    // 验证主机名不包含非法字符
    EXPECT_EQ(hostName.find('\0'), std::string::npos) << "主机名不应包含空字符";
    EXPECT_EQ(hostName.find('\n'), std::string::npos) << "主机名不应包含换行符";
    EXPECT_EQ(hostName.find('\r'), std::string::npos) << "主机名不应包含回车符";
}

TEST_F(SystemTest, GetHostNameConsistency)
{
    std::string hostName1 = getHostName();
    std::string hostName2 = getHostName();
    EXPECT_EQ(hostName1, hostName2) << "主机名应保持一致";
}

TEST_F(SystemTest, GetHostNameFormat)
{
    std::string hostName = getHostName();
    EXPECT_FALSE(hostName.empty()) << "主机名不应为空";

    // 验证主机名格式（通常只包含字母、数字、连字符和点）
    for (char c : hostName)
    {
        bool isValidChar = std::isalnum(c) || c == '-' || c == '.';
        EXPECT_TRUE(isValidChar) << "主机名包含非法字符: " << c;
    }
}

// ========================================
// 用户名测试
// ========================================

TEST_F(SystemTest, GetUserName)
{
    std::string userName = getUserName();
    EXPECT_FALSE(userName.empty()) << "用户名不应为空";

    // 验证用户名不包含非法字符
    EXPECT_EQ(userName.find('\0'), std::string::npos) << "用户名不应包含空字符";
    EXPECT_EQ(userName.find('\n'), std::string::npos) << "用户名不应包含换行符";
    EXPECT_EQ(userName.find('\r'), std::string::npos) << "用户名不应包含回车符";
}

TEST_F(SystemTest, GetUserNameConsistency)
{
    std::string userName1 = getUserName();
    std::string userName2 = getUserName();
    EXPECT_EQ(userName1, userName2) << "用户名应保持一致";
}

TEST_F(SystemTest, GetUserNameFormat)
{
    std::string userName = getUserName();
    EXPECT_FALSE(userName.empty()) << "用户名不应为空";

    // 验证用户名格式（通常只包含字母、数字、下划线和连字符）
    for (char c : userName)
    {
        bool isValidChar = std::isalnum(c) || c == '_' || c == '-';
        EXPECT_TRUE(isValidChar) << "用户名包含非法字符: " << c;
    }
}

// ========================================
// 处理器核心数测试
// ========================================

TEST_F(SystemTest, GetProcessorCount)
{
    int count = getProcessorCount();
    EXPECT_GT(count, 0) << "处理器核心数应大于 0";
    EXPECT_LE(count, 256) << "处理器核心数应在合理范围内";
}

TEST_F(SystemTest, GetProcessorCountConsistency)
{
    int count1 = getProcessorCount();
    int count2 = getProcessorCount();
    EXPECT_EQ(count1, count2) << "处理器核心数应保持一致";
}

// ========================================
// 内存信息测试
// ========================================

TEST_F(SystemTest, GetTotalMemory)
{
    size_t totalMemory = getTotalMemory();
    EXPECT_GT(totalMemory, 0) << "总内存应大于 0";

    // 验证内存大小在合理范围内（至少 1MB）
    EXPECT_GT(totalMemory, 1024 * 1024) << "总内存应至少为 1MB";
}

TEST_F(SystemTest, GetTotalMemoryConsistency)
{
    size_t totalMemory1 = getTotalMemory();
    size_t totalMemory2 = getTotalMemory();
    EXPECT_EQ(totalMemory1, totalMemory2) << "总内存应保持一致";
}

TEST_F(SystemTest, GetAvailableMemory)
{
    size_t availableMemory = getAvailableMemory();
    EXPECT_GT(availableMemory, 0) << "可用内存应大于 0";

    // 验证可用内存不超过总内存
    size_t totalMemory = getTotalMemory();
    EXPECT_LE(availableMemory, totalMemory) << "可用内存不应超过总内存";
}

TEST_F(SystemTest, GetAvailableMemoryConsistency)
{
    size_t availableMemory1 = getAvailableMemory();
    size_t availableMemory2 = getAvailableMemory();
    // 注意：可用内存可能会变化，所以这里只验证两次获取都成功
    EXPECT_GT(availableMemory1, 0) << "第一次获取可用内存应成功";
    EXPECT_GT(availableMemory2, 0) << "第二次获取可用内存应成功";
}

TEST_F(SystemTest, MemoryConsistency)
{
    size_t totalMemory = getTotalMemory();
    size_t availableMemory = getAvailableMemory();

    EXPECT_GT(totalMemory, 0) << "总内存应大于 0";
    EXPECT_GT(availableMemory, 0) << "可用内存应大于 0";
    EXPECT_LE(availableMemory, totalMemory) << "可用内存不应超过总内存";
}

// ========================================
// 系统信息一致性测试
// ========================================

TEST_F(SystemTest, SystemInfoConsistency)
{
    std::string osName = getOSName();
    std::string osVersion = getOSVersion();
    std::string arch = getOSArchitecture();
    std::string hostName = getHostName();
    std::string userName = getUserName();
    int processorCount = getProcessorCount();
    size_t totalMemory = getTotalMemory();
    size_t availableMemory = getAvailableMemory();

    EXPECT_FALSE(osName.empty()) << "操作系统名称不应为空";
    EXPECT_FALSE(arch.empty()) << "系统架构不应为空";
    EXPECT_FALSE(hostName.empty()) << "主机名不应为空";
    EXPECT_FALSE(userName.empty()) << "用户名不应为空";
    EXPECT_GT(processorCount, 0) << "处理器核心数应大于 0";
    EXPECT_GT(totalMemory, 0) << "总内存应大于 0";
    EXPECT_GT(availableMemory, 0) << "可用内存应大于 0";
    EXPECT_LE(availableMemory, totalMemory) << "可用内存不应超过总内存";
}

// ========================================
// 系统信息稳定性测试
// ========================================

TEST_F(SystemTest, SystemInfoStability)
{
    // 多次获取系统信息，验证稳定性
    for (int i = 0; i < 10; ++i)
    {
        std::string osName = getOSName();
        std::string arch = getOSArchitecture();
        std::string hostName = getHostName();
        std::string userName = getUserName();
        int processorCount = getProcessorCount();
        size_t totalMemory = getTotalMemory();

        EXPECT_FALSE(osName.empty()) << "操作系统名称不应为空";
        EXPECT_FALSE(arch.empty()) << "系统架构不应为空";
        EXPECT_FALSE(hostName.empty()) << "主机名不应为空";
        EXPECT_FALSE(userName.empty()) << "用户名不应为空";
        EXPECT_GT(processorCount, 0) << "处理器核心数应大于 0";
        EXPECT_GT(totalMemory, 0) << "总内存应大于 0";
    }
}

// ========================================
// 平台特定测试
// ========================================

#if defined(_WIN32) || defined(_WIN64)
TEST_F(SystemTest, WindowsSpecific)
{
    std::string osName = getOSName();
    EXPECT_EQ(osName, "Windows") << "Windows 系统应返回 Windows";
}
#elif defined(__linux__)
TEST_F(SystemTest, LinuxSpecific)
{
    std::string osName = getOSName();
    EXPECT_EQ(osName, "Linux") << "Linux 系统应返回 Linux";
}
#elif defined(__APPLE__)
TEST_F(SystemTest, MacOSSpecific)
{
    std::string osName = getOSName();
    EXPECT_EQ(osName, "macOS") << "macOS 系统应返回 macOS";
}
#endif
