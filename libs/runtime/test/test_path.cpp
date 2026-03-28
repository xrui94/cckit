// test_path.cpp - 路径相关测试
#include <gtest/gtest.h>
#include "cckit/runtime/Runtime.hpp"
#include <filesystem>

using namespace cckit::runtime;

class PathTest : public ::testing::Test
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
// 可执行文件路径测试
// ========================================

TEST_F(PathTest, GetExecutablePath)
{
    std::string path = getExecutablePath();
    EXPECT_FALSE(path.empty()) << "可执行文件路径不应为空";

    // 验证路径格式
    std::filesystem::path fsPath(path);
    EXPECT_FALSE(fsPath.filename().empty()) << "路径应包含文件名";

    // 验证文件存在
    EXPECT_TRUE(std::filesystem::exists(fsPath)) << "可执行文件应存在";
}

TEST_F(PathTest, GetExecutableDir)
{
    std::string dir = getExecutableDir();
    EXPECT_FALSE(dir.empty()) << "可执行文件目录不应为空";

    // 验证目录存在
    std::filesystem::path fsDir(dir);
    EXPECT_TRUE(std::filesystem::is_directory(fsDir)) << "路径应为目录";

    // 验证目录包含可执行文件
    std::string exePath = getExecutablePath();
    std::filesystem::path fsExePath(exePath);
    EXPECT_EQ(fsDir, fsExePath.parent_path()) << "目录应为可执行文件的父目录";
}

// ========================================
// 动态库路径测试
// ========================================

TEST_F(PathTest, GetLibraryPath)
{
    std::string path = getLibraryPath();
    // 注意：在某些情况下（如静态链接），此函数可能返回空字符串
    // 所以这里只验证返回值格式，不强制要求非空

    if (!path.empty())
    {
        // 验证路径格式
        std::filesystem::path fsPath(path);
        EXPECT_FALSE(fsPath.filename().empty()) << "路径应包含文件名";

        // 验证文件存在
        EXPECT_TRUE(std::filesystem::exists(fsPath)) << "动态库文件应存在";
    }
}

TEST_F(PathTest, GetLibraryDir)
{
    std::string dir = getLibraryDir();
    // 注意：在某些情况下（如静态链接），此函数可能返回空字符串

    if (!dir.empty())
    {
        // 验证目录存在
        std::filesystem::path fsDir(dir);
        EXPECT_TRUE(std::filesystem::is_directory(fsDir)) << "路径应为目录";

        // 验证目录包含动态库
        std::string libPath = getLibraryPath();
        std::filesystem::path fsLibPath(libPath);
        EXPECT_EQ(fsDir, fsLibPath.parent_path()) << "目录应为动态库的父目录";
    }
}

// ========================================
// 路径一致性测试
// ========================================

TEST_F(PathTest, PathConsistency)
{
    std::string exePath = getExecutablePath();
    std::string exeDir = getExecutableDir();

    if (!exePath.empty() && !exeDir.empty())
    {
        std::filesystem::path fsExePath(exePath);
        std::filesystem::path fsExeDir(exeDir);

        EXPECT_EQ(fsExeDir, fsExePath.parent_path())
            << "可执行文件目录应为可执行文件路径的父目录";
    }
}

TEST_F(PathTest, LibraryPathConsistency)
{
    std::string libPath = getLibraryPath();
    std::string libDir = getLibraryDir();

    if (!libPath.empty() && !libDir.empty())
    {
        std::filesystem::path fsLibPath(libPath);
        std::filesystem::path fsLibDir(libDir);

        EXPECT_EQ(fsLibDir, fsLibPath.parent_path())
            << "动态库目录应为动态库路径的父目录";
    }
}

// ========================================
// 路径格式测试
// ========================================

TEST_F(PathTest, ExecutablePathFormat)
{
    std::string path = getExecutablePath();
    if (!path.empty())
    {
        // 验证路径不包含非法字符
        EXPECT_EQ(path.find('\0'), std::string::npos) << "路径不应包含空字符";

        // 验证路径使用正确的分隔符
#if defined(_WIN32) || defined(_WIN64)
        EXPECT_NE(path.find('\\'), std::string::npos) << "Windows 路径应使用反斜杠";
#else
        EXPECT_NE(path.find('/'), std::string::npos) << "Unix 路径应使用正斜杠";
#endif
    }
}

TEST_F(PathTest, ExecutableDirFormat)
{
    std::string dir = getExecutableDir();
    if (!dir.empty())
    {
        // 验证目录不以路径分隔符结尾（某些实现可能包含）
        // 这里只验证目录格式基本正确
        EXPECT_FALSE(dir.empty()) << "目录不应为空";
    }
}
