// test_distributions.cpp - 分布测试（正态分布、布尔值等）
#include <gtest/gtest.h>
#include "cckit/random/Random.hpp"
#include <vector>
#include <cmath>
#include <algorithm>

using namespace cckit::random;

class DistributionTest : public ::testing::Test
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
// 正态分布测试
// ========================================

TEST_F(DistributionTest, NormalFloatBasic)
{
    RandomGenerator rng;
    const float mean = 0.0f;
    const float stddev = 1.0f;

    // 生成样本
    std::vector<float> samples;
    const int numSamples = 10000;
    for (int i = 0; i < numSamples; ++i) {
        samples.push_back(rng.nextNormal(mean, stddev));
    }

    // 计算样本均值
    float sum = 0.0f;
    for (float s : samples) {
        sum += s;
    }
    float sampleMean = sum / numSamples;

    // 计算样本标准差
    float variance = 0.0f;
    for (float s : samples) {
        variance += (s - sampleMean) * (s - sampleMean);
    }
    float sampleStddev = std::sqrt(variance / numSamples);

    // 验证均值和标准差接近理论值
    EXPECT_NEAR(sampleMean, mean, 0.1f) << "样本均值应接近理论均值";
    EXPECT_NEAR(sampleStddev, stddev, 0.1f) << "样本标准差应接近理论标准差";
}

TEST_F(DistributionTest, NormalDoubleBasic)
{
    RandomGenerator rng;
    const double mean = 100.0;
    const double stddev = 15.0;

    // 生成样本
    std::vector<double> samples;
    const int numSamples = 10000;
    for (int i = 0; i < numSamples; ++i) {
        samples.push_back(rng.nextNormal(mean, stddev));
    }

    // 计算样本均值
    double sum = 0.0;
    for (double s : samples) {
        sum += s;
    }
    double sampleMean = sum / numSamples;

    // 计算样本标准差
    double variance = 0.0;
    for (double s : samples) {
        variance += (s - sampleMean) * (s - sampleMean);
    }
    double sampleStddev = std::sqrt(variance / numSamples);

    // 验证均值和标准差接近理论值
    EXPECT_NEAR(sampleMean, mean, 1.0) << "样本均值应接近理论均值";
    EXPECT_NEAR(sampleStddev, stddev, 1.0) << "样本标准差应接近理论标准差";
}

TEST_F(DistributionTest, NormalFloatCustomParams)
{
    RandomGenerator rng;
    const float mean = 50.0f;
    const float stddev = 10.0f;

    // 生成样本
    std::vector<float> samples;
    const int numSamples = 10000;
    for (int i = 0; i < numSamples; ++i) {
        samples.push_back(rng.nextNormal(mean, stddev));
    }

    // 计算样本均值
    float sum = 0.0f;
    for (float s : samples) {
        sum += s;
    }
    float sampleMean = sum / numSamples;

    // 验证均值接近理论值
    EXPECT_NEAR(sampleMean, mean, 1.0f) << "样本均值应接近理论均值";
}

// ========================================
// 布尔值测试
// ========================================

TEST_F(DistributionTest, BoolDefault)
{
    RandomGenerator rng;
    const int samples = 10000;
    int trueCount = 0;

    for (int i = 0; i < samples; ++i) {
        if (rng.nextBool()) {
            trueCount++;
        }
    }

    // 50% 概率，允许 5% 误差
    double ratio = static_cast<double>(trueCount) / samples;
    EXPECT_NEAR(ratio, 0.5, 0.05) << "布尔值应大致均匀分布";
}

TEST_F(DistributionTest, BoolWithProbability)
{
    RandomGenerator rng;
    const int samples = 10000;
    const float p = 0.3f;
    int trueCount = 0;

    for (int i = 0; i < samples; ++i) {
        if (rng.nextBool(p)) {
            trueCount++;
        }
    }

    // 30% 概率，允许 5% 误差
    double ratio = static_cast<double>(trueCount) / samples;
    EXPECT_NEAR(ratio, p, 0.05) << "布尔值概率应接近指定值";
}

TEST_F(DistributionTest, BoolWithHighProbability)
{
    RandomGenerator rng;
    const int samples = 10000;
    const float p = 0.9f;
    int trueCount = 0;

    for (int i = 0; i < samples; ++i) {
        if (rng.nextBool(p)) {
            trueCount++;
        }
    }

    // 90% 概率，允许 5% 误差
    double ratio = static_cast<double>(trueCount) / samples;
    EXPECT_NEAR(ratio, p, 0.05) << "布尔值概率应接近指定值";
}

TEST_F(DistributionTest, BoolWithLowProbability)
{
    RandomGenerator rng;
    const int samples = 10000;
    const float p = 0.1f;
    int trueCount = 0;

    for (int i = 0; i < samples; ++i) {
        if (rng.nextBool(p)) {
            trueCount++;
        }
    }

    // 10% 概率，允许 5% 误差
    double ratio = static_cast<double>(trueCount) / samples;
    EXPECT_NEAR(ratio, p, 0.05) << "布尔值概率应接近指定值";
}

TEST_F(DistributionTest, BoolExtremeProbabilities)
{
    RandomGenerator rng;

    // 概率为 0 时应总是返回 false
    for (int i = 0; i < 100; ++i) {
        EXPECT_FALSE(rng.nextBool(0.0f));
    }

    // 概率为 1 时应总是返回 true
    for (int i = 0; i < 100; ++i) {
        EXPECT_TRUE(rng.nextBool(1.0f));
    }
}

// ========================================
// 洗牌算法测试
// ========================================

TEST_F(DistributionTest, ShuffleBasic)
{
    RandomGenerator rng;
    std::vector<int> original = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> shuffled = original;

    rng.shuffle(shuffled);

    // 检查元素数量相同
    EXPECT_EQ(shuffled.size(), original.size());

    // 检查所有元素都存在
    for (int val : original) {
        EXPECT_TRUE(std::find(shuffled.begin(), shuffled.end(), val) != shuffled.end())
            << "元素 " << val << " 应该在洗牌后的数组中";
    }
}

TEST_F(DistributionTest, ShufflePreservesElements)
{
    RandomGenerator rng;
    std::vector<int> original = {1, 2, 3, 4, 5};
    std::vector<int> shuffled = original;

    rng.shuffle(shuffled);

    // 排序后应与原数组相同
    std::sort(shuffled.begin(), shuffled.end());
    EXPECT_EQ(shuffled, original) << "洗牌不应改变元素集合";
}

TEST_F(DistributionTest, ShuffleSingleElement)
{
    RandomGenerator rng;
    std::vector<int> vec = {42};
    rng.shuffle(vec);

    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], 42);
}

TEST_F(DistributionTest, ShuffleEmpty)
{
    RandomGenerator rng;
    std::vector<int> vec;
    rng.shuffle(vec);

    EXPECT_TRUE(vec.empty());
}

TEST_F(DistributionTest, ShuffleRandomness)
{
    RandomGenerator rng;
    std::vector<int> original = {1, 2, 3, 4, 5};

    // 多次洗牌，检查是否产生不同的排列
    std::vector<std::vector<int>> results;
    for (int i = 0; i < 100; ++i) {
        std::vector<int> shuffled = original;
        rng.shuffle(shuffled);
        results.push_back(shuffled);
    }

    // 统计不同排列的数量
    std::sort(results.begin(), results.end());
    auto last = std::unique(results.begin(), results.end());
    int uniqueCount = std::distance(results.begin(), last);

    // 应该产生多种不同的排列
    EXPECT_GT(uniqueCount, 10) << "洗牌应产生多种不同的排列";
}
