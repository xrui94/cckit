// test_time.cpp - 时间相关测试
#include <gtest/gtest.h>
#include "cckit/runtime/Runtime.hpp"
#include <thread>
#include <chrono>

using namespace cckit::runtime;

class TimeTest : public ::testing::Test
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
// 进程运行时间测试
// ========================================

TEST_F(TimeTest, GetProcessUptime)
{
    uint64_t uptime = getProcessUptime();
    EXPECT_GT(uptime, 0) << "进程运行时间应大于 0";

    // 验证时间在合理范围内（进程刚启动，不应超过 1 小时）
    EXPECT_LT(uptime, 3600000) << "进程运行时间应在合理范围内";
}

TEST_F(TimeTest, GetProcessUptimeMonotonic)
{
    uint64_t uptime1 = getProcessUptime();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    uint64_t uptime2 = getProcessUptime();

    EXPECT_GT(uptime2, uptime1) << "进程运行时间应单调递增";

    // 验证时间差在合理范围内（约 100ms）
    uint64_t diff = uptime2 - uptime1;
    EXPECT_GE(diff, 50) << "时间差应至少为 50ms";
    EXPECT_LE(diff, 500) << "时间差应不超过 500ms";
}

TEST_F(TimeTest, GetProcessUptimeConsistency)
{
    // 多次获取进程运行时间，验证单调性
    uint64_t prevUptime = getProcessUptime();

    for (int i = 0; i < 10; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        uint64_t uptime = getProcessUptime();
        EXPECT_GE(uptime, prevUptime) << "进程运行时间应单调递增";
        prevUptime = uptime;
    }
}

// ========================================
// 系统时间测试
// ========================================

TEST_F(TimeTest, GetSystemTime)
{
    uint64_t systemTime = getSystemTime();
    EXPECT_GT(systemTime, 0) << "系统时间应大于 0";

    // 验证时间在合理范围内（Unix 时间戳，毫秒）
    // 2020-01-01 00:00:00 UTC = 1577836800000 ms
    EXPECT_GT(systemTime, 1577836800000ULL) << "系统时间应在 2020 年之后";
}

TEST_F(TimeTest, GetSystemTimeMonotonic)
{
    uint64_t systemTime1 = getSystemTime();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    uint64_t systemTime2 = getSystemTime();

    EXPECT_GT(systemTime2, systemTime1) << "系统时间应单调递增";

    // 验证时间差在合理范围内（约 100ms）
    uint64_t diff = systemTime2 - systemTime1;
    EXPECT_GE(diff, 50) << "时间差应至少为 50ms";
    EXPECT_LE(diff, 500) << "时间差应不超过 500ms";
}

TEST_F(TimeTest, GetSystemTimeConsistency)
{
    // 多次获取系统时间，验证单调性
    uint64_t prevSystemTime = getSystemTime();

    for (int i = 0; i < 10; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        uint64_t systemTime = getSystemTime();
        EXPECT_GE(systemTime, prevSystemTime) << "系统时间应单调递增";
        prevSystemTime = systemTime;
    }
}

// ========================================
// 单调时间测试
// ========================================

TEST_F(TimeTest, GetMonotonicTime)
{
    uint64_t monotonicTime = getMonotonicTime();
    EXPECT_GT(monotonicTime, 0) << "单调时间应大于 0";
}

TEST_F(TimeTest, GetMonotonicTimeMonotonic)
{
    uint64_t monotonicTime1 = getMonotonicTime();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    uint64_t monotonicTime2 = getMonotonicTime();

    EXPECT_GT(monotonicTime2, monotonicTime1) << "单调时间应单调递增";

    // 验证时间差在合理范围内（约 100ms）
    uint64_t diff = monotonicTime2 - monotonicTime1;
    EXPECT_GE(diff, 50) << "时间差应至少为 50ms";
    EXPECT_LE(diff, 500) << "时间差应不超过 500ms";
}

TEST_F(TimeTest, GetMonotonicTimeConsistency)
{
    // 多次获取单调时间，验证单调性
    uint64_t prevMonotonicTime = getMonotonicTime();

    for (int i = 0; i < 10; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        uint64_t monotonicTime = getMonotonicTime();
        EXPECT_GE(monotonicTime, prevMonotonicTime) << "单调时间应单调递增";
        prevMonotonicTime = monotonicTime;
    }
}

// ========================================
// 时间函数对比测试
// ========================================

TEST_F(TimeTest, SystemTimeVsMonotonicTime)
{
    uint64_t systemTime1 = getSystemTime();
    uint64_t monotonicTime1 = getMonotonicTime();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    uint64_t systemTime2 = getSystemTime();
    uint64_t monotonicTime2 = getMonotonicTime();

    uint64_t systemDiff = systemTime2 - systemTime1;
    uint64_t monotonicDiff = monotonicTime2 - monotonicTime1;

    // 两个时间差应该相近
    EXPECT_GE(systemDiff, monotonicDiff * 0.5) << "系统时间差不应小于单调时间差的一半";
    EXPECT_LE(systemDiff, monotonicDiff * 2) << "系统时间差不应大于单调时间差的两倍";
}

TEST_F(TimeTest, ProcessUptimeVsMonotonicTime)
{
    uint64_t processUptime = getProcessUptime();
    uint64_t monotonicTime = getMonotonicTime();

    // 进程运行时间应该小于或等于单调时间
    EXPECT_LE(processUptime, monotonicTime) << "进程运行时间应小于或等于单调时间";
}

// ========================================
// 时间精度测试
// ========================================

TEST_F(TimeTest, TimePrecision)
{
    // 测试时间精度（毫秒级）
    uint64_t time1 = getMonotonicTime();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    uint64_t time2 = getMonotonicTime();

    EXPECT_GE(time2, time1) << "时间应单调递增";

    // 注意：由于系统调度，1ms 的睡眠可能不会精确反映在时间差中
    // 这里只验证时间差不为负
}

TEST_F(TimeTest, TimeResolution)
{
    // 测试时间分辨率
    std::vector<uint64_t> times;
    for (int i = 0; i < 100; ++i)
    {
        times.push_back(getMonotonicTime());
    }

    // 验证时间值在合理范围内
    for (size_t i = 1; i < times.size(); ++i)
    {
        EXPECT_GE(times[i], times[i - 1]) << "时间应单调递增";
    }
}

// ========================================
// 时间稳定性测试
// ========================================

TEST_F(TimeTest, TimeStability)
{
    // 长时间测试，验证时间函数的稳定性
    uint64_t startTime = getMonotonicTime();
    uint64_t startSystemTime = getSystemTime();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    uint64_t endTime = getMonotonicTime();
    uint64_t endSystemTime = getSystemTime();

    uint64_t monotonicDiff = endTime - startTime;
    uint64_t systemDiff = endSystemTime - startSystemTime;

    // 验证时间差在合理范围内（约 500ms）
    EXPECT_GE(monotonicDiff, 400) << "单调时间差应至少为 400ms";
    EXPECT_LE(monotonicDiff, 600) << "单调时间差应不超过 600ms";
    EXPECT_GE(systemDiff, 400) << "系统时间差应至少为 400ms";
    EXPECT_LE(systemDiff, 600) << "系统时间差应不超过 600ms";
}

// ========================================
// 时间边界测试
// ========================================

TEST_F(TimeTest, TimeBoundary)
{
    // 测试时间函数在边界情况下的行为
    uint64_t time1 = getSystemTime();
    uint64_t time2 = getSystemTime();

    // 两次连续调用的时间差应该很小
    uint64_t diff = time2 - time1;
    EXPECT_LT(diff, 100) << "连续调用的时间差应小于 100ms";
}

// ========================================
// 时间一致性测试
// ========================================

TEST_F(TimeTest, TimeConsistency)
{
    uint64_t processUptime = getProcessUptime();
    uint64_t systemTime = getSystemTime();
    uint64_t monotonicTime = getMonotonicTime();

    EXPECT_GT(processUptime, 0) << "进程运行时间应大于 0";
    EXPECT_GT(systemTime, 0) << "系统时间应大于 0";
    EXPECT_GT(monotonicTime, 0) << "单调时间应大于 0";

    // 验证时间关系
    EXPECT_LE(processUptime, monotonicTime) << "进程运行时间应小于或等于单调时间";
    EXPECT_GT(systemTime, 1577836800000ULL) << "系统时间应在 2020 年之后";
}

// ========================================
// 时间函数性能测试
// ========================================

TEST_F(TimeTest, TimePerformance)
{
    // 测试时间函数的性能
    const int iterations = 10000;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i)
    {
        getMonotonicTime();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // 验证性能：10000 次调用应在 100ms 内完成
    EXPECT_LT(duration.count(), 100000) << "时间函数性能应足够好";
}
