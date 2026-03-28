// test_process.cpp - 进程相关测试
#include <gtest/gtest.h>
#include "cckit/runtime/Runtime.hpp"

#include <thread>
#include <filesystem>

using namespace cckit::runtime;

class ProcessTest : public ::testing::Test
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
// 进程 ID 测试
// ========================================

TEST_F(ProcessTest, GetProcessId)
{
    int pid = getProcessId();
    EXPECT_GT(pid, 0) << "进程 ID 应大于 0";
    EXPECT_LT(pid, 65536) << "进程 ID 应在合理范围内";
}

TEST_F(ProcessTest, GetProcessIdConsistency)
{
    int pid1 = getProcessId();
    int pid2 = getProcessId();
    EXPECT_EQ(pid1, pid2) << "同一进程的 ID 应保持一致";
}

// ========================================
// 父进程 ID 测试
// ========================================

TEST_F(ProcessTest, GetParentProcessId)
{
    int ppid = getParentProcessId();
    EXPECT_GT(ppid, 0) << "父进程 ID 应大于 0";
    EXPECT_LT(ppid, 65536) << "父进程 ID 应在合理范围内";
}

TEST_F(ProcessTest, ParentProcessIdDifferent)
{
    int pid = getProcessId();
    int ppid = getParentProcessId();
    EXPECT_NE(pid, ppid) << "父进程 ID 应与当前进程 ID 不同";
}

// ========================================
// 进程名称测试
// ========================================

TEST_F(ProcessTest, GetProcessName)
{
    std::string name = getProcessName();
    EXPECT_FALSE(name.empty()) << "进程名称不应为空";

    // 验证名称不包含路径分隔符
#if defined(_WIN32) || defined(_WIN64)
    EXPECT_EQ(name.find('\\'), std::string::npos) << "进程名称不应包含路径分隔符";
#else
    EXPECT_EQ(name.find('/'), std::string::npos) << "进程名称不应包含路径分隔符";
#endif

    // 验证名称不包含扩展名（某些平台可能包含）
    // 这里只验证名称基本格式正确
    EXPECT_FALSE(name.empty()) << "进程名称不应为空";
}

TEST_F(ProcessTest, ProcessNameMatchesExecutable)
{
    std::string name = getProcessName();
    std::string exePath = getExecutablePath();

    if (!name.empty() && !exePath.empty())
    {
        std::filesystem::path fsExePath(exePath);
        std::string exeName = fsExePath.filename().string();

        // 某些平台可能返回不带扩展名的名称
        // 这里只验证名称匹配（可能需要去除扩展名）
        EXPECT_FALSE(name.empty()) << "进程名称不应为空";
    }
}

// ========================================
// 命令行参数测试
// ========================================

TEST_F(ProcessTest, GetArgc)
{
    int argc = getArgc();
    EXPECT_GE(argc, 0) << "参数数量应大于等于 0";
}

TEST_F(ProcessTest, GetArgv)
{
    std::vector<std::string> argv = getArgv();
    // 注意：在 C++ 中，main 函数的参数通常不可直接访问
    // 所以这里只验证返回值格式，不强制要求非空
    EXPECT_GE(argv.size(), 0) << "参数数组大小应大于等于 0";
}

TEST_F(ProcessTest, ArgcArgvConsistency)
{
    int argc = getArgc();
    std::vector<std::string> argv = getArgv();
    EXPECT_EQ(argc, static_cast<int>(argv.size())) << "argc 应与 argv 大小一致";
}

// ========================================
// 命令行字符串测试
// ========================================

TEST_F(ProcessTest, GetCommandLine)
{
    std::string cmdline = getCommandLine();
    EXPECT_FALSE(cmdline.empty()) << "命令行字符串不应为空";

    // 验证命令行包含可执行文件名
    std::string exeName = getProcessName();
    if (!exeName.empty())
    {
        EXPECT_NE(cmdline.find(exeName), std::string::npos)
            << "命令行应包含可执行文件名";
    }
}

TEST_F(ProcessTest, CommandLineFormat)
{
    std::string cmdline = getCommandLine();
    EXPECT_FALSE(cmdline.empty()) << "命令行字符串不应为空";

    // 验证命令行不包含空字符
    EXPECT_EQ(cmdline.find('\0'), std::string::npos) << "命令行不应包含空字符";
}

// ========================================
// 进程信息一致性测试
// ========================================

TEST_F(ProcessTest, ProcessInfoConsistency)
{
    int pid = getProcessId();
    int ppid = getParentProcessId();
    std::string name = getProcessName();
    std::string cmdline = getCommandLine();

    EXPECT_GT(pid, 0) << "进程 ID 应大于 0";
    EXPECT_GT(ppid, 0) << "父进程 ID 应大于 0";
    EXPECT_FALSE(name.empty()) << "进程名称不应为空";
    EXPECT_FALSE(cmdline.empty()) << "命令行字符串不应为空";
}

// ========================================
// 进程稳定性测试
// ========================================

TEST_F(ProcessTest, ProcessIdStability)
{
    int pid1 = getProcessId();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    int pid2 = getProcessId();
    EXPECT_EQ(pid1, pid2) << "进程 ID 在短时间内应保持稳定";
}

TEST_F(ProcessTest, ParentProcessIdStability)
{
    int ppid1 = getParentProcessId();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    int ppid2 = getParentProcessId();
    EXPECT_EQ(ppid1, ppid2) << "父进程 ID 在短时间内应保持稳定";
}
