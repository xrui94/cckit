// test_uuid_performance.cpp - UUID 性能测试
#include <cckit/uuid/Uuid.hpp>

#include <gtest/gtest.h>
#include <chrono>

TEST(UuidPerformance, Generate)
{
    const int count = 10000;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; ++i) {
        auto uuid = cckit::uuid::Uuid::generate();
        (void)uuid;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "生成 " << count << " 个 UUID 耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每个 UUID 耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;
    SUCCEED();
}

TEST(UuidPerformance, ToString)
{
    const int count = 10000;
    std::vector<cckit::uuid::Uuid> uuids;
    for (int i = 0; i < count; ++i)
        uuids.push_back(cckit::uuid::Uuid::generate());

    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& uuid : uuids) {
        auto str = uuid.toString();
        (void)str;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "转换 " << count << " 个 UUID 为字符串耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次转换耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;
    SUCCEED();
}

TEST(UuidPerformance, FromString)
{
    const int count = 10000;
    std::string validStr = "550e8400-e29b-41d4-a716-446655440000";

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; ++i) {
        auto uuid = cckit::uuid::Uuid::fromString(validStr);
        (void)uuid;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "解析 " << count << " 个 UUID 字符串耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次解析耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;
    SUCCEED();
}

TEST(UuidPerformance, Comparison)
{
    const int count = 100000;
    std::vector<cckit::uuid::Uuid> uuids;
    for (int i = 0; i < count; ++i)
        uuids.push_back(cckit::uuid::Uuid::generate());

    auto start = std::chrono::high_resolution_clock::now();
    int equalCount = 0;
    for (int i = 1; i < count; ++i) {
        if (uuids[i] == uuids[i - 1])
            equalCount++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "比较 " << count << " 个 UUID 耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次比较耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;
    EXPECT_EQ(equalCount, 0);
    SUCCEED();
}

TEST(UuidPerformance, Hashing)
{
    const int count = 100000;
    std::vector<cckit::uuid::Uuid> uuids;
    for (int i = 0; i < count; ++i)
        uuids.push_back(cckit::uuid::Uuid::generate());

    std::hash<cckit::uuid::Uuid> hasher;
    auto start = std::chrono::high_resolution_clock::now();
    size_t hashSum = 0;
    for (const auto& uuid : uuids)
        hashSum += hasher(uuid);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "哈希 " << count << " 个 UUID 耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "哈希总和: " << hashSum << " (用于防止优化)" << std::endl;
    std::cout << "平均每次哈希耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;
    SUCCEED();
}
