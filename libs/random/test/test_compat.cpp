// test_compat.cpp - 兼容层 API 测试
#include <gtest/gtest.h>
#include "cckit/random/Random.hpp"
#include <vector>
#include <algorithm>

// 使用旧版命名空间
using namespace cdt::common::random;

class CompatTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

// ========================================
// getRandomEngine 测试
// ========================================

TEST_F(CompatTest, GetRandomEngine)
{
    std::mt19937& engine1 = getRandomEngine();
    std::mt19937& engine2 = getRandomEngine();

    // 应该返回同一个引擎实例
    EXPECT_EQ(&engine1, &engine2) << "getRandomEngine 应返回同一实例";
}

TEST_F(CompatTest, RandomEngineGeneratesValues)
{
    std::mt19937& engine = getRandomEngine();

    // 生成一些随机值
    uint32_t val1 = engine();
    uint32_t val2 = engine();
    uint32_t val3 = engine();

    // 这些值应该不同（概率极高）
    EXPECT_NE(val1, val2);
    EXPECT_NE(val2, val3);
    EXPECT_NE(val1, val3);
}

// ========================================
// weightedRandomIndex 测试
// ========================================

TEST_F(CompatTest, WeightedRandomIndexBasic)
{
    std::vector<float> weights = {1.0f, 2.0f, 3.0f};

    const int samples = 10000;
    std::vector<int> counts(weights.size(), 0);

    for (int i = 0; i < samples; ++i) {
        size_t idx = weightedRandomIndex(weights);
        EXPECT_LT(idx, weights.size()) << "索引应在有效范围内";
        counts[idx]++;
    }

    // 检查分布比例: 1:2:3
    float expected0 = samples * 1.0f / 6.0f;
    float expected1 = samples * 2.0f / 6.0f;
    float expected2 = samples * 3.0f / 6.0f;

    EXPECT_NEAR(counts[0], expected0, expected0 * 0.2f);
    EXPECT_NEAR(counts[1], expected1, expected1 * 0.2f);
    EXPECT_NEAR(counts[2], expected2, expected2 * 0.2f);
}

TEST_F(CompatTest, WeightedRandomIndexUniform)
{
    std::vector<float> weights = {1.0f, 1.0f, 1.0f, 1.0f};

    const int samples = 10000;
    std::vector<int> counts(weights.size(), 0);

    for (int i = 0; i < samples; ++i) {
        size_t idx = weightedRandomIndex(weights);
        counts[idx]++;
    }

    int expected = samples / 4;
    for (size_t i = 0; i < weights.size(); ++i) {
        EXPECT_NEAR(counts[i], expected, expected * 0.2f);
    }
}

TEST_F(CompatTest, WeightedRandomIndexSingle)
{
    std::vector<float> weights = {5.0f};

    for (int i = 0; i < 100; ++i) {
        size_t idx = weightedRandomIndex(weights);
        EXPECT_EQ(idx, 0);
    }
}

TEST_F(CompatTest, WeightedRandomIndexEmpty)
{
    std::vector<float> weights;
    size_t idx = weightedRandomIndex(weights);
    EXPECT_EQ(idx, 0) << "空权重数组应返回 0";
}

// ========================================
// weightedRandomChoice 测试
// ========================================

TEST_F(CompatTest, WeightedRandomChoiceBasic)
{
    std::vector<std::string> items = {"a", "b", "c"};
    std::vector<float> weights = {1.0f, 2.0f, 3.0f};

    const int samples = 10000;
    std::vector<int> counts(items.size(), 0);

    for (int i = 0; i < samples; ++i) {
        std::string choice = weightedRandomChoice(items, weights);
        if (choice == "a") counts[0]++;
        else if (choice == "b") counts[1]++;
        else if (choice == "c") counts[2]++;
    }

    float expected0 = samples * 1.0f / 6.0f;
    float expected1 = samples * 2.0f / 6.0f;
    float expected2 = samples * 3.0f / 6.0f;

    EXPECT_NEAR(counts[0], expected0, expected0 * 0.2f);
    EXPECT_NEAR(counts[1], expected1, expected1 * 0.2f);
    EXPECT_NEAR(counts[2], expected2, expected2 * 0.2f);
}

TEST_F(CompatTest, WeightedRandomChoiceIntegers)
{
    std::vector<int> items = {10, 20, 30};
    std::vector<float> weights = {1.0f, 1.0f, 1.0f};

    const int samples = 1000;
    for (int i = 0; i < samples; ++i) {
        int choice = weightedRandomChoice(items, weights);
        EXPECT_TRUE(choice == 10 || choice == 20 || choice == 30);
    }
}

TEST_F(CompatTest, WeightedRandomChoiceSingle)
{
    std::vector<int> items = {42};
    std::vector<float> weights = {1.0f};

    for (int i = 0; i < 100; ++i) {
        int choice = weightedRandomChoice(items, weights);
        EXPECT_EQ(choice, 42);
    }
}

TEST_F(CompatTest, WeightedRandomChoiceEmpty)
{
    std::vector<int> items;
    std::vector<float> weights;

    int choice = weightedRandomChoice(items, weights);
    EXPECT_EQ(choice, 0) << "空数组应返回默认值";
}

// ========================================
// randomChoice 测试
// ========================================

TEST_F(CompatTest, RandomChoiceBasic)
{
    std::vector<int> items = {1, 2, 3, 4, 5};

    const int samples = 10000;
    std::vector<int> counts(items.size(), 0);

    for (int i = 0; i < samples; ++i) {
        int choice = randomChoice(items);
        auto it = std::find(items.begin(), items.end(), choice);
        EXPECT_NE(it, items.end());
        counts[std::distance(items.begin(), it)]++;
    }

    int expected = samples / items.size();
    for (size_t i = 0; i < items.size(); ++i) {
        EXPECT_NEAR(counts[i], expected, expected * 0.2f);
    }
}

TEST_F(CompatTest, RandomChoiceStrings)
{
    std::vector<std::string> items = {"red", "green", "blue"};

    const int samples = 1000;
    for (int i = 0; i < samples; ++i) {
        std::string choice = randomChoice(items);
        EXPECT_TRUE(choice == "red" || choice == "green" || choice == "blue");
    }
}

TEST_F(CompatTest, RandomChoiceSingle)
{
    std::vector<int> items = {99};

    for (int i = 0; i < 100; ++i) {
        int choice = randomChoice(items);
        EXPECT_EQ(choice, 99);
    }
}

TEST_F(CompatTest, RandomChoiceEmpty)
{
    std::vector<int> items;
    int choice = randomChoice(items);
    EXPECT_EQ(choice, 0) << "空数组应返回默认值";
}

// ========================================
// 混合使用测试
// ========================================

TEST_F(CompatTest, MixedUsage)
{
    std::vector<int> items = {1, 2, 3, 4, 5};
    std::vector<float> weights = {1.0f, 2.0f, 3.0f, 2.0f, 1.0f};

    // 混合使用各种函数
    auto& engine = getRandomEngine();
    (void)engine();

    size_t idx = weightedRandomIndex(weights);
    EXPECT_LT(idx, weights.size());

    int choice1 = weightedRandomChoice(items, weights);
    EXPECT_TRUE(std::find(items.begin(), items.end(), choice1) != items.end());

    int choice2 = randomChoice(items);
    EXPECT_TRUE(std::find(items.begin(), items.end(), choice2) != items.end());
}

// ========================================
// 线程安全测试（基础）
// ========================================

TEST_F(CompatTest, ThreadLocalEngine)
{
    // 每个线程应该有独立的引擎实例
    // 这里只测试单线程情况
    std::mt19937& engine1 = getRandomEngine();
    std::mt19937& engine2 = getRandomEngine();

    EXPECT_EQ(&engine1, &engine2) << "同一线程应返回同一引擎";
}
