// test_environment.cpp - 环境变量测试
#include <gtest/gtest.h>
#include "cckit/runtime/Runtime.hpp"
#include <algorithm>

using namespace cckit::runtime;

class EnvironmentTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // 保存原始环境变量
        m_testVarName = "CCKIT_RUNTIME_TEST_VAR";
        m_originalValue = getEnv(m_testVarName);
    }

    void TearDown() override
    {
        // 恢复原始环境变量
        if (m_originalValue.empty())
        {
            unsetEnv(m_testVarName);
        }
        else
        {
            setEnv(m_testVarName, m_originalValue);
        }
    }

    std::string m_testVarName;
    std::string m_originalValue;
};

// ========================================
// 获取环境变量测试
// ========================================

TEST_F(EnvironmentTest, GetEnv)
{
    // 测试获取存在的环境变量
    std::string path = getEnv("PATH");
    EXPECT_FALSE(path.empty()) << "PATH 环境变量应存在";

    // 测试获取不存在的环境变量
    std::string nonexistent = getEnv("CCKIT_RUNTIME_NONEXISTENT_VAR_12345");
    EXPECT_TRUE(nonexistent.empty()) << "不存在的环境变量应返回空字符串";
}

TEST_F(EnvironmentTest, GetEnvCaseSensitivity)
{
    // 设置测试环境变量
    setEnv(m_testVarName, "test_value");

    // 验证大小写敏感（Unix）或不敏感（Windows）
    std::string value1 = getEnv(m_testVarName);
    EXPECT_EQ(value1, "test_value") << "应能获取刚设置的环境变量";

#if defined(_WIN32) || defined(_WIN64)
    // Windows 环境变量不区分大小写
    std::string upperName = m_testVarName;
    std::transform(upperName.begin(), upperName.end(), upperName.begin(), ::toupper);
    std::string value2 = getEnv(upperName);
    EXPECT_EQ(value2, "test_value") << "Windows 环境变量不区分大小写";
#endif
}

// ========================================
// 获取环境变量（带默认值）测试
// ========================================

TEST_F(EnvironmentTest, GetEnvOrDefault)
{
    // 测试获取存在的环境变量
    setEnv(m_testVarName, "test_value");
    std::string value = getEnvOrDefault(m_testVarName, "default");
    EXPECT_EQ(value, "test_value") << "应返回实际值而非默认值";

    // 测试获取不存在的环境变量
    std::string defaultValue = getEnvOrDefault("CCKIT_RUNTIME_NONEXISTENT_VAR_12345", "default");
    EXPECT_EQ(defaultValue, "default") << "应返回默认值";
}

TEST_F(EnvironmentTest, GetEnvOrDefaultEmptyString)
{
    // 测试空字符串作为默认值
    std::string value = getEnvOrDefault("CCKIT_RUNTIME_NONEXISTENT_VAR_12345", "");
    EXPECT_TRUE(value.empty()) << "空字符串应作为有效的默认值";
}

// ========================================
// 设置环境变量测试
// ========================================

TEST_F(EnvironmentTest, SetEnv)
{
    // 测试设置新环境变量
    bool result = setEnv(m_testVarName, "test_value");
    EXPECT_TRUE(result) << "设置环境变量应成功";

    std::string value = getEnv(m_testVarName);
    EXPECT_EQ(value, "test_value") << "应能获取刚设置的环境变量";
}

TEST_F(EnvironmentTest, SetEnvUpdate)
{
    // 测试更新已存在的环境变量
    setEnv(m_testVarName, "old_value");
    bool result = setEnv(m_testVarName, "new_value");
    EXPECT_TRUE(result) << "更新环境变量应成功";

    std::string value = getEnv(m_testVarName);
    EXPECT_EQ(value, "new_value") << "应获取更新后的值";
}

TEST_F(EnvironmentTest, SetEnvEmptyValue)
{
    // 测试设置空值
    bool result = setEnv(m_testVarName, "");
    EXPECT_TRUE(result) << "设置空值应成功";

    std::string value = getEnv(m_testVarName);
    EXPECT_TRUE(value.empty()) << "应能获取空值";
}

TEST_F(EnvironmentTest, SetEnvSpecialCharacters)
{
    // 测试设置包含特殊字符的值
    std::string specialValue = "test;value|with&special$chars";
    bool result = setEnv(m_testVarName, specialValue);
    EXPECT_TRUE(result) << "设置包含特殊字符的值应成功";

    std::string value = getEnv(m_testVarName);
    EXPECT_EQ(value, specialValue) << "应能正确获取包含特殊字符的值";
}

// ========================================
// 删除环境变量测试
// ========================================

TEST_F(EnvironmentTest, UnsetEnv)
{
    // 先设置环境变量
    setEnv(m_testVarName, "test_value");

    // 删除环境变量
    bool result = unsetEnv(m_testVarName);
    EXPECT_TRUE(result) << "删除环境变量应成功";

    // 验证已删除
    std::string value = getEnv(m_testVarName);
    EXPECT_TRUE(value.empty()) << "删除后应返回空字符串";
}

TEST_F(EnvironmentTest, UnsetEnvNonexistent)
{
    // 删除不存在的环境变量
    bool result = unsetEnv("CCKIT_RUNTIME_NONEXISTENT_VAR_12345");
    EXPECT_TRUE(result) << "删除不存在的环境变量也应成功";
}

// ========================================
// PATH 环境变量测试
// ========================================

TEST_F(EnvironmentTest, GetPathEnv)
{
    std::vector<std::string> paths = getPathEnv();
    EXPECT_FALSE(paths.empty()) << "PATH 环境变量不应为空";

    // 验证每个路径都不为空
    for (const auto& path : paths)
    {
        EXPECT_FALSE(path.empty()) << "PATH 中的每个路径都不应为空";
    }
}

TEST_F(EnvironmentTest, GetPathEnvFormat)
{
    std::vector<std::string> paths = getPathEnv();

    // 验证路径格式
    for (const auto& path : paths)
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

// ========================================
// 获取所有环境变量测试
// ========================================

TEST_F(EnvironmentTest, GetAllEnv)
{
    std::map<std::string, std::string> envMap = getAllEnv();
    EXPECT_FALSE(envMap.empty()) << "环境变量映射不应为空";

    // 验证 PATH 存在（Windows 上可能是 Path 或 PATH）
    bool foundPath = false;
    for (const auto& [key, value] : envMap)
    {
        std::string upperKey = key;
        std::transform(upperKey.begin(), upperKey.end(), upperKey.begin(), ::toupper);
        if (upperKey == "PATH")
        {
            foundPath = true;
            break;
        }
    }
    EXPECT_TRUE(foundPath) << "PATH 环境变量应存在";

    // 验证每个键值对都不为空
    for (const auto& [key, value] : envMap)
    {
        EXPECT_FALSE(key.empty()) << "环境变量名不应为空";
        // 值可以为空
    }
}

TEST_F(EnvironmentTest, GetAllEnvConsistency)
{
    // 设置测试环境变量
    setEnv(m_testVarName, "test_value");

    std::map<std::string, std::string> envMap = getAllEnv();

    // 验证能获取刚设置的环境变量
    auto it = envMap.find(m_testVarName);
    EXPECT_NE(it, envMap.end()) << "应能获取刚设置的环境变量";
    if (it != envMap.end())
    {
        EXPECT_EQ(it->second, "test_value") << "值应匹配";
    }
}

// ========================================
// 环境变量操作顺序测试
// ========================================

TEST_F(EnvironmentTest, EnvOperationSequence)
{
    // 设置 -> 获取 -> 更新 -> 获取 -> 删除 -> 获取
    EXPECT_TRUE(setEnv(m_testVarName, "value1"));
    EXPECT_EQ(getEnv(m_testVarName), "value1");

    EXPECT_TRUE(setEnv(m_testVarName, "value2"));
    EXPECT_EQ(getEnv(m_testVarName), "value2");

    EXPECT_TRUE(unsetEnv(m_testVarName));
    EXPECT_TRUE(getEnv(m_testVarName).empty());
}

// ========================================
// 环境变量并发测试
// ========================================

TEST_F(EnvironmentTest, EnvConcurrentAccess)
{
    // 多次设置和获取
    for (int i = 0; i < 100; ++i)
    {
        std::string value = "value_" + std::to_string(i);
        EXPECT_TRUE(setEnv(m_testVarName, value));
        EXPECT_EQ(getEnv(m_testVarName), value);
    }
}
