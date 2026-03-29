// test_weighted.cpp - 加权随机测试
#include <gtest/gtest.h>
#include "cckit/random/Random.hpp"
#include <vector>
#include <cmath>

using namespace cckit::random;

class WeightedTest : public ::testing::Test
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
// 加权索引测试
// ========================================

TEST_F(WeightedTest, WeightedIndexBasic)
{
    RandomGenerator rng;
    std::vector<float> weights = {1.0f, 2.0f, 3.0f};

    // 生成大量样本
    const int samples = 10000;
    std::vector<int> counts(weights.size(), 0);

    for (int i = 0; i < samples; ++i) {
        size_t idx = rng.weightedIndex(weights);
        EXPECT_LT(idx, weights.size()) << "索引应在有效范围内";
        counts[idx]++;
    }

    // 检查分布比例
    // 权重比例: 1:2:3，即 1/6, 2/6, 3/6
    float expected0 = samples * 1.0f / 6.0f;
    float expected1 = samples * 2.0f / 6.0f;
    float expected2 = samples * 3.0f / 6.0f;

    EXPECT_NEAR(counts[0], expected0, expected0 * 0.2f) << "索引 0 的频率应接近 1/6";
    EXPECT_NEAR(counts[1], expected1, expected1 * 0.2f) << "索引 1 的频率应接近 2/6";
    EXPECT_NEAR(counts[2], expected2, expected2 * 0.2f) << "索引 2 的频率应接近 3/6";
}

TEST_F(WeightedTest, WeightedIndexUniform)
{
    RandomGenerator rng;
    std::vector<float> weights = {1.0f, 1.0f, 1.0f, 1.0f};

    const int samples = 10000;
    std::vector<int> counts(weights.size(), 0);

    for (int i = 0; i < samples; ++i) {
        size_t idx = rng.weightedIndex(weights);
        counts[idx]++;
    }

    // 均匀分布，每个索引约占 25%
    int expected = samples / 4;
    for (size_t i = 0; i < weights.size(); ++i) {
        EXPECT_NEAR(counts[i], expected, expected * 0.2f)
            << "索引 " << i << " 的频率应接近 25%";
    }
}

TEST_F(WeightedTest, WeightedIndexSingle)
{
    RandomGenerator rng;
    std::vector<float> weights = {5.0f};

    for (int i = 0; i < 100; ++i) {
        size_t idx = rng.weightedIndex(weights);
        EXPECT_EQ(idx, 0) << "只有一个权重时，索引应总是 0";
    }
}

TEST_F(WeightedTest, WeightedIndexZeroWeights)
{
    RandomGenerator rng;
    std::vector<float> weights = {0.0f, 1.0f, 0.0f};

    const int samples = 1000;
    for (int i = 0; i < samples; ++i) {
        size_t idx = rng.weightedIndex(weights);
        EXPECT_EQ(idx, 1) << "只有索引 1 有非零权重";
    }
}

TEST_F(WeightedTest, WeightedIndexDouble)
{
    RandomGenerator rng;
    std::vector<double> weights = {1.0, 3.0};

    const int samples = 10000;
    std::vector<int> counts(weights.size(), 0);

    for (int i = 0; i < samples; ++i) {
        size_t idx = rng.weightedIndex(weights);
        counts[idx]++;
    }

    // 权重比例: 1:3，即 25%, 75%
    double expected0 = samples * 0.25;
    double expected1 = samples * 0.75;

    EXPECT_NEAR(counts[0], expected0, expected0 * 0.2);
    EXPECT_NEAR(counts[1], expected1, expected1 * 0.2);
}

// ========================================
// 加权选择测试
// ========================================

TEST_F(WeightedTest, WeightedChoiceBasic)
{
    RandomGenerator rng;
    std::vector<std::string> items = {"apple", "banana", "cherry"};
    std::vector<float> weights = {1.0f, 2.0f, 3.0f};

    const int samples = 10000;
    std::vector<int> counts(items.size(), 0);

    for (int i = 0; i < samples; ++i) {
        std::string choice = rng.weightedChoice(items, weights);
        if (choice == "apple") counts[0]++;
        else if (choice == "banana") counts[1]++;
        else if (choice == "cherry") counts[2]++;
    }

    // 检查分布比例
    float expected0 = samples * 1.0f / 6.0f;
    float expected1 = samples * 2.0f / 6.0f;
    float expected2 = samples * 3.0f / 6.0f;

    EXPECT_NEAR(counts[0], expected0, expected0 * 0.2f);
    EXPECT_NEAR(counts[1], expected1, expected1 * 0.2f);
    EXPECT_NEAR(counts[2], expected2, expected2 * 0.2f);
}

TEST_F(WeightedTest, WeightedChoiceIntegers)
{
    RandomGenerator rng;
    std::vector<int> items = {10, 20, 30, 40};
    std::vector<float> weights = {1.0f, 1.0f, 1.0f, 1.0f};

    const int samples = 1000;
    for (int i = 0; i < samples; ++i) {
        int choice = rng.weightedChoice(items, weights);
        EXPECT_TRUE(choice == 10 || choice == 20 || choice == 30 || choice == 40)
            << "选择的值应在候选列表中";
    }
}

// ========================================
// 普通选择测试
// ========================================

TEST_F(WeightedTest, ChoiceBasic)
{
    RandomGenerator rng;
    std::vector<int> items = {1, 2, 3, 4, 5};

    const int samples = 10000;
    std::vector<int> counts(items.size(), 0);

    for (int i = 0; i < samples; ++i) {
        int choice = rng.choice(items);
        auto it = std::find(items.begin(), items.end(), choice);
        EXPECT_NE(it, items.end()) << "选择的值应在候选列表中";
        counts[std::distance(items.begin(), it)]++;
    }

    // 检查均匀分布
    int expected = samples / items.size();
    for (size_t i = 0; i < items.size(); ++i) {
        EXPECT_NEAR(counts[i], expected, expected * 0.2f)
            << "每个元素应被均匀选择";
    }
}

TEST_F(WeightedTest, ChoiceSingle)
{
    RandomGenerator rng;
    std::vector<int> items = {42};

    for (int i = 0; i < 100; ++i) {
        int choice = rng.choice(items);
        EXPECT_EQ(choice, 42) << "只有一个元素时，应总是选择它";
    }
}

TEST_F(WeightedTest, ChoiceStrings)
{
    RandomGenerator rng;
    std::vector<std::string> items = {"red", "green", "blue"};

    const int samples = 1000;
    for (int i = 0; i < samples; ++i) {
        std::string choice = rng.choice(items);
        EXPECT_TRUE(choice == "red" || choice == "green" || choice == "blue")
            << "选择的值应在候选列表中";
    }
}

// ========================================
// 边界情况测试
// ========================================

TEST_F(WeightedTest, EmptyWeights)
{
    RandomGenerator rng;
    std::vector<float> weights;

    size_t idx = rng.weightedIndex(weights);
    EXPECT_EQ(idx, 0) << "空权重数组应返回 0";
}

TEST_F(WeightedTest, EmptyItems)
{
    RandomGenerator rng;
    std::vector<int> items;
    std::vector<float> weights;

    int choice = rng.weightedChoice(items, weights);
    EXPECT_EQ(choice, 0) << "空项目数组应返回默认值";

    int choice2 = rng.choice(items);
    EXPECT_EQ(choice2, 0) << "空项目数组应返回默认值";
}

TEST_F(WeightedTest, VerySmallWeights)
{
    RandomGenerator rng;
    std::vector<float> weights = {0.001f, 0.001f, 0.998f};

    const int samples = 10000;
    std::vector<int> counts(weights.size(), 0);

    for (int i = 0; i < samples; ++i) {
        size_t idx = rng.weightedIndex(weights);
        counts[idx]++;
    }

    // 索引 2 应占约 99.8%
    EXPECT_NEAR(counts[2], samples * 0.998f, samples * 0.05f);
}

TEST_F(WeightedTest, LargeWeights)
{
    RandomGenerator rng;
    std::vector<float> weights = {1000.0f, 1.0f};

    const int samples = 10000;
    std::vector<int> counts(weights.size(), 0);

    for (int i = 0; i < samples; ++i) {
        size_t idx = rng.weightedIndex(weights);
        counts[idx]++;
    }

    // 索引 0 应占约 99.9%
    EXPECT_NEAR(counts[0], samples * 0.999f, samples * 0.05f);
}
