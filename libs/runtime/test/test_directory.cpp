// test_directory.cpp - 目录相关测试
#include <gtest/gtest.h>
#include "cckit/runtime/Runtime.hpp"
#include <filesystem>
#include <fstream>

using namespace cckit::runtime;

class DirectoryTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // 测试前的设置
        m_originalCwd = getCurrentWorkingDirectory();
    }

    void TearDown() override
    {
        // 测试后的清理
        setCurrentWorkingDirectory(m_originalCwd);

        // 清理临时文件和目录
        if (!m_tempDir.empty() && std::filesystem::exists(m_tempDir))
        {
            std::filesystem::remove_all(m_tempDir);
        }
        if (!m_tempFile.empty() && std::filesystem::exists(m_tempFile))
        {
            std::filesystem::remove(m_tempFile);
        }
    }

    std::string m_originalCwd;
    std::string m_tempDir;
    std::string m_tempFile;
};

// ========================================
// 当前工作目录测试
// ========================================

TEST_F(DirectoryTest, GetCurrentWorkingDirectory)
{
    std::string cwd = getCurrentWorkingDirectory();
    EXPECT_FALSE(cwd.empty()) << "当前工作目录不应为空";

    // 验证目录存在
    std::filesystem::path fsCwd(cwd);
    EXPECT_TRUE(std::filesystem::exists(fsCwd)) << "当前工作目录应存在";
    EXPECT_TRUE(std::filesystem::is_directory(fsCwd)) << "当前工作目录应为目录";
}

TEST_F(DirectoryTest, GetCurrentWorkingDirectoryFormat)
{
    std::string cwd = getCurrentWorkingDirectory();
    EXPECT_FALSE(cwd.empty()) << "当前工作目录不应为空";

    // 验证路径格式
    std::filesystem::path fsCwd(cwd);
    EXPECT_FALSE(fsCwd.empty()) << "路径不应为空";

    // 验证路径使用正确的分隔符
#if defined(_WIN32) || defined(_WIN64)
    EXPECT_NE(cwd.find('\\'), std::string::npos) << "Windows 路径应使用反斜杠";
#else
    EXPECT_NE(cwd.find('/'), std::string::npos) << "Unix 路径应使用正斜杠";
#endif
}

// ========================================
// 设置工作目录测试
// ========================================

TEST_F(DirectoryTest, SetCurrentWorkingDirectory)
{
    std::string originalCwd = getCurrentWorkingDirectory();
    std::string tempDir = getTempDirectory();

    bool result = setCurrentWorkingDirectory(tempDir);
    EXPECT_TRUE(result) << "设置工作目录应成功";

    std::string newCwd = getCurrentWorkingDirectory();
    EXPECT_EQ(newCwd, tempDir) << "工作目录应已更改";

    // 恢复原始目录
    setCurrentWorkingDirectory(originalCwd);
}

TEST_F(DirectoryTest, SetCurrentWorkingDirectoryNonexistent)
{
    std::string originalCwd = getCurrentWorkingDirectory();
    std::string nonexistentDir = "/path/to/nonexistent/directory/12345";

    bool result = setCurrentWorkingDirectory(nonexistentDir);
    EXPECT_FALSE(result) << "设置不存在的目录应失败";

    std::string cwd = getCurrentWorkingDirectory();
    EXPECT_EQ(cwd, originalCwd) << "工作目录应保持不变";
}

TEST_F(DirectoryTest, SetCurrentWorkingDirectoryRelative)
{
    std::string originalCwd = getCurrentWorkingDirectory();

    // 设置为相对路径
    bool result = setCurrentWorkingDirectory(".");
    EXPECT_TRUE(result) << "设置为当前目录应成功";

    std::string cwd = getCurrentWorkingDirectory();
    EXPECT_EQ(cwd, originalCwd) << "工作目录应保持不变";

    // 恢复原始目录
    setCurrentWorkingDirectory(originalCwd);
}

// ========================================
// 临时目录测试
// ========================================

TEST_F(DirectoryTest, GetTempDirectory)
{
    std::string tempDir = getTempDirectory();
    EXPECT_FALSE(tempDir.empty()) << "临时目录不应为空";

    // 验证目录存在
    std::filesystem::path fsTempDir(tempDir);
    EXPECT_TRUE(std::filesystem::exists(fsTempDir)) << "临时目录应存在";
    EXPECT_TRUE(std::filesystem::is_directory(fsTempDir)) << "临时目录应为目录";
}

TEST_F(DirectoryTest, GetTempDirectoryWritable)
{
    std::string tempDir = getTempDirectory();

    // 尝试在临时目录中创建文件
    std::filesystem::path testFile = std::filesystem::path(tempDir) / "test_write.txt";
    std::ofstream file(testFile);
    EXPECT_TRUE(file.is_open()) << "临时目录应可写";

    if (file.is_open())
    {
        file << "test";
        file.close();
        std::filesystem::remove(testFile);
    }
}

// ========================================
// 创建临时目录测试
// ========================================

TEST_F(DirectoryTest, CreateTempDirectory)
{
    std::string tempDir = createTempDirectory("cckit_test_");
    EXPECT_FALSE(tempDir.empty()) << "创建的临时目录不应为空";

    // 保存路径以便清理
    m_tempDir = tempDir;

    // 验证目录存在
    std::filesystem::path fsTempDir(tempDir);
    EXPECT_TRUE(std::filesystem::exists(fsTempDir)) << "临时目录应存在";
    EXPECT_TRUE(std::filesystem::is_directory(fsTempDir)) << "临时目录应为目录";

    // 验证目录可写
    std::filesystem::path testFile = fsTempDir / "test.txt";
    std::ofstream file(testFile);
    EXPECT_TRUE(file.is_open()) << "临时目录应可写";

    if (file.is_open())
    {
        file << "test";
        file.close();
    }
}

TEST_F(DirectoryTest, CreateTempDirectoryUnique)
{
    // 创建多个临时目录，验证它们是唯一的
    std::string tempDir1 = createTempDirectory("cckit_test_");
    std::string tempDir2 = createTempDirectory("cckit_test_");

    m_tempDir = tempDir1; // 保存第一个以便清理

    EXPECT_FALSE(tempDir1.empty()) << "第一个临时目录不应为空";
    EXPECT_FALSE(tempDir2.empty()) << "第二个临时目录不应为空";
    EXPECT_NE(tempDir1, tempDir2) << "临时目录应是唯一的";

    // 清理第二个目录
    if (!tempDir2.empty() && std::filesystem::exists(tempDir2))
    {
        std::filesystem::remove_all(tempDir2);
    }
}

TEST_F(DirectoryTest, CreateTempDirectoryPrefix)
{
    std::string prefix = "cckit_test_prefix_";
    std::string tempDir = createTempDirectory(prefix);

    m_tempDir = tempDir;

    EXPECT_FALSE(tempDir.empty()) << "临时目录不应为空";

    // 验证目录名包含前缀（Windows 上前缀可能被截断为 3 个字符）
    std::filesystem::path fsTempDir(tempDir);
    std::string dirName = fsTempDir.filename().string();
    
#if defined(_WIN32) || defined(_WIN64)
    // Windows: GetTempFileNameW 只支持最多 3 个字符的前缀
    std::string expectedPrefix = prefix.substr(0, 3);  // "cck"
    EXPECT_TRUE(dirName.find(expectedPrefix) == 0 || dirName.find(expectedPrefix) != std::string::npos)
        << "目录名应包含前缀前3个字符";
#else
    // Linux/macOS: 支持完整前缀
    EXPECT_TRUE(dirName.find(prefix) == 0 || dirName.find(prefix) != std::string::npos)
        << "目录名应包含前缀";
#endif
}

// ========================================
// 创建临时文件测试
// ========================================

TEST_F(DirectoryTest, CreateTempFile)
{
    std::string tempFile = createTempFile("cckit_test_");
    EXPECT_FALSE(tempFile.empty()) << "创建的临时文件不应为空";

    // 保存路径以便清理
    m_tempFile = tempFile;

    // 验证文件存在
    std::filesystem::path fsTempFile(tempFile);
    EXPECT_TRUE(std::filesystem::exists(fsTempFile)) << "临时文件应存在";
    EXPECT_TRUE(std::filesystem::is_regular_file(fsTempFile)) << "临时文件应为普通文件";

    // 验证文件可写
    std::ofstream file(fsTempFile, std::ios::app);
    EXPECT_TRUE(file.is_open()) << "临时文件应可写";

    if (file.is_open())
    {
        file << "test";
        file.close();
    }
}

TEST_F(DirectoryTest, CreateTempFileUnique)
{
    // 创建多个临时文件，验证它们是唯一的
    std::string tempFile1 = createTempFile("cckit_test_");
    std::string tempFile2 = createTempFile("cckit_test_");

    m_tempFile = tempFile1; // 保存第一个以便清理

    EXPECT_FALSE(tempFile1.empty()) << "第一个临时文件不应为空";
    EXPECT_FALSE(tempFile2.empty()) << "第二个临时文件不应为空";
    EXPECT_NE(tempFile1, tempFile2) << "临时文件应是唯一的";

    // 清理第二个文件
    if (!tempFile2.empty() && std::filesystem::exists(tempFile2))
    {
        std::filesystem::remove(tempFile2);
    }
}

TEST_F(DirectoryTest, CreateTempFilePrefix)
{
    std::string prefix = "cckit_test_prefix_";
    std::string tempFile = createTempFile(prefix);

    m_tempFile = tempFile;

    EXPECT_FALSE(tempFile.empty()) << "临时文件不应为空";

    // 验证文件名包含前缀（Windows 上前缀可能被截断为 3 个字符）
    std::filesystem::path fsTempFile(tempFile);
    std::string fileName = fsTempFile.filename().string();
    
#if defined(_WIN32) || defined(_WIN64)
    // Windows: GetTempFileNameW 只支持最多 3 个字符的前缀
    std::string expectedPrefix = prefix.substr(0, 3);  // "cck"
    EXPECT_TRUE(fileName.find(expectedPrefix) == 0 || fileName.find(expectedPrefix) != std::string::npos)
        << "文件名应包含前缀前3个字符";
#else
    // Linux/macOS: 支持完整前缀
    EXPECT_TRUE(fileName.find(prefix) == 0 || fileName.find(prefix) != std::string::npos)
        << "文件名应包含前缀";
#endif
}

// ========================================
// 目录操作一致性测试
// ========================================

TEST_F(DirectoryTest, DirectoryOperationConsistency)
{
    std::string tempDir = getTempDirectory();
    std::string createdDir = createTempDirectory("cckit_test_");

    m_tempDir = createdDir;

    // 验证创建的目录在临时目录中
    std::filesystem::path fsTempDir(tempDir);
    std::filesystem::path fsCreatedDir(createdDir);
    EXPECT_EQ(fsTempDir, fsCreatedDir.parent_path())
        << "创建的临时目录应在系统临时目录中";
}

// ========================================
// 目录路径格式测试
// ========================================

TEST_F(DirectoryTest, DirectoryPathFormat)
{
    std::string cwd = getCurrentWorkingDirectory();
    std::string tempDir = getTempDirectory();

    // 验证路径不包含非法字符
    EXPECT_EQ(cwd.find('\0'), std::string::npos) << "路径不应包含空字符";
    EXPECT_EQ(tempDir.find('\0'), std::string::npos) << "路径不应包含空字符";

    // 验证路径使用正确的分隔符
#if defined(_WIN32) || defined(_WIN64)
    EXPECT_NE(cwd.find('\\'), std::string::npos) << "Windows 路径应使用反斜杠";
    EXPECT_NE(tempDir.find('\\'), std::string::npos) << "Windows 路径应使用反斜杠";
#else
    EXPECT_NE(cwd.find('/'), std::string::npos) << "Unix 路径应使用正斜杠";
    EXPECT_NE(tempDir.find('/'), std::string::npos) << "Unix 路径应使用正斜杠";
#endif
}
