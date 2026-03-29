// test_random_generator.cpp - 随机数生成器基础测试
#include <gtest/gtest.h>
#include "cckit/random/Random.hpp"
#include <vector>
#include <algorithm>
#include <cmath>

using namespace cckit::random;

class RandomGeneratorTest : public ::testing::Test
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
// 构造和生命周期测试
// ========================================

TEST_F(RandomGeneratorTest, CreateDefault)
{
    RandomGenerator rng;
    EXPECT_NE(rng.handle(), nullptr) << "随机数生成器句柄不应为空";
}

TEST_F(RandomGeneratorTest, CreateWithType)
{
    RandomGenerator rng1(CCKIT_RNG_MT19937);
    RandomGenerator rng2(CCKIT_RNG_MT19937_64);
    RandomGenerator rng3(CCKIT_RNG_MINSTD_RAND);
    RandomGenerator rng4(CCKIT_RNG_KNUTH_B);

    EXPECT_NE(rng1.handle(), nullptr);
    EXPECT_NE(rng2.handle(), nullptr);
    EXPECT_NE(rng3.handle(), nullptr);
    EXPECT_NE(rng4.handle(), nullptr);
}

TEST_F(RandomGeneratorTest, CreateWithSeed)
{
    RandomGenerator rng1(CCKIT_RNG_MT19937, 12345);
    RandomGenerator rng2(CCKIT_RNG_MT19937, 12345);

    // 相同种子应产生相同序列
    EXPECT_EQ(rng1.nextInt(0, 100), rng2.nextInt(0, 100));
    EXPECT_EQ(rng1.nextInt(0, 100), rng2.nextInt(0, 100));
    EXPECT_EQ(rng1.nextFloat(0.0f, 1.0f), rng2.nextFloat(0.0f, 1.0f));
}

TEST_F(RandomGeneratorTest, MoveSemantics)
{
    RandomGenerator rng1(CCKIT_RNG_MT19937, 42);
    int val1 = rng1.nextInt(0, 100);

    RandomGenerator rng2 = std::move(rng1);
    EXPECT_EQ(rng1.handle(), nullptr) << "移动后原对象句柄应为空";
    EXPECT_NE(rng2.handle(), nullptr) << "移动后新对象句柄不应为空";
}

TEST_F(RandomGeneratorTest, Reseed)
{
    RandomGenerator rng(CCKIT_RNG_MT19937, 100);
    int val1 = rng.nextInt(0, 1000);

    rng.seed(100);
    int val2 = rng.nextInt(0, 1000);

    EXPECT_EQ(val1, val2) << "重新设置相同种子应产生相同序列";
}

// ========================================
// 整数随机数测试
// ========================================

TEST_F(RandomGeneratorTest, NextIntRange)
{
    RandomGenerator rng;
    const int min = 10;
    const int max = 20;

    for (int i = 0; i < 1000; ++i) {
        int val = rng.nextInt(min, max);
        EXPECT_GE(val, min) << "随机整数不应小于最小值";
        EXPECT_LE(val, max) << "随机整数不应大于最大值";
    }
}

TEST_F(RandomGeneratorTest, NextIntDistribution)
{
    RandomGenerator rng;
    const int min = 0;
    const int max = 9;
    const int samples = 10000;

    std::vector<int> counts(max - min + 1, 0);
    for (int i = 0; i < samples; ++i) {
        int val = rng.nextInt(min, max);
        counts[val - min]++;
    }

    // 检查每个值都出现过（概率很高）
    for (int i = 0; i <= max - min; ++i) {
        EXPECT_GT(counts[i], 0) << "值 " << (min + i) << " 应该出现过";
    }

    // 检查分布大致均匀（每个值约占 10%，允许 5% 误差）
    int expected = samples / (max - min + 1);
    for (int i = 0; i <= max - min; ++i) {
        EXPECT_NEAR(counts[i], expected, expected * 0.5)
            << "值 " << (min + i) << " 的分布应大致均匀";
    }
}

TEST_F(RandomGeneratorTest, NextUIntRange)
{
    RandomGenerator rng;
    const uint32_t min = 100;
    const uint32_t max = 200;

    for (int i = 0; i < 1000; ++i) {
        uint32_t val = rng.nextUInt(min, max);
        EXPECT_GE(val, min);
        EXPECT_LE(val, max);
    }
}

TEST_F(RandomGeneratorTest, NextInt64Range)
{
    RandomGenerator rng;
    const int64_t min = -1000000LL;
    const int64_t max = 1000000LL;

    for (int i = 0; i < 1000; ++i) {
        int64_t val = rng.nextInt64(min, max);
        EXPECT_GE(val, min);
        EXPECT_LE(val, max);
    }
}

TEST_F(RandomGeneratorTest, NextUInt64Range)
{
    RandomGenerator rng;
    const uint64_t min = 0ULL;
    const uint64_t max = 1000000000ULL;

    for (int i = 0; i < 1000; ++i) {
        uint64_t val = rng.nextUInt64(min, max);
        EXPECT_GE(val, min);
        EXPECT_LE(val, max);
    }
}

// ========================================
// 浮点随机数测试
// ========================================

TEST_F(RandomGeneratorTest, NextFloatRange)
{
    RandomGenerator rng;
    const float min = -10.0f;
    const float max = 10.0f;

    for (int i = 0; i < 1000; ++i) {
        float val = rng.nextFloat(min, max);
        EXPECT_GE(val, min) << "随机浮点数不应小于最小值";
        EXPECT_LT(val, max) << "随机浮点数应小于最大值（半开区间）";
    }
}

TEST_F(RandomGeneratorTest, NextUnitFloat)
{
    RandomGenerator rng;

    for (int i = 0; i < 1000; ++i) {
        float val = rng.nextUnitFloat();
        EXPECT_GE(val, 0.0f) << "单位随机浮点数不应小于 0";
        EXPECT_LT(val, 1.0f) << "单位随机浮点数应小于 1";
    }
}

TEST_F(RandomGeneratorTest, NextDoubleRange)
{
    RandomGenerator rng;
    const double min = -100.0;
    const double max = 100.0;

    for (int i = 0; i < 1000; ++i) {
        double val = rng.nextDouble(min, max);
        EXPECT_GE(val, min);
        EXPECT_LT(val, max);
    }
}

TEST_F(RandomGeneratorTest, NextUnitDouble)
{
    RandomGenerator rng;

    for (int i = 0; i < 1000; ++i) {
        double val = rng.nextUnitDouble();
        EXPECT_GE(val, 0.0);
        EXPECT_LT(val, 1.0);
    }
}

TEST_F(RandomGeneratorTest, FloatDistribution)
{
    RandomGenerator rng;
    const int samples = 10000;
    double sum = 0.0;

    for (int i = 0; i < samples; ++i) {
        sum += rng.nextUnitDouble();
    }

    double mean = sum / samples;
    EXPECT_NEAR(mean, 0.5, 0.05) << "均匀分布 [0,1) 的均值应接近 0.5";
}

// ========================================
// 全局随机数生成器测试
// ========================================

TEST_F(RandomGeneratorTest, GlobalRng)
{
    RandomGenerator& rng1 = globalRng();
    RandomGenerator& rng2 = globalRng();

    EXPECT_EQ(&rng1, &rng2) << "全局随机数生成器应为同一实例";
}

TEST_F(RandomGeneratorTest, GlobalConvenienceFunctions)
{
    int val1 = randInt(0, 100);
    float val2 = randFloat(0.0f, 1.0f);
    float val3 = randUnitFloat();

    EXPECT_GE(val1, 0);
    EXPECT_LE(val1, 100);
    EXPECT_GE(val2, 0.0f);
    EXPECT_LT(val2, 1.0f);
    EXPECT_GE(val3, 0.0f);
    EXPECT_LT(val3, 1.0f);
}
