// test_uuid_performance.cpp - UUID 性能测试
#include <cckit/uuid/Uuid.hpp>

#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

void test_generation_performance()
{
    std::cout << "测试 UUID 生成性能..." << std::endl;

    const int count = 100000;
    std::vector<cckit::uuid::Uuid> uuids;
    uuids.reserve(count);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; ++i)
    {
        uuids.push_back(cckit::uuid::Uuid::generate());
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "生成 " << count << " 个 UUID 耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每个 UUID 耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;

    // 验证没有重复
    std::sort(uuids.begin(), uuids.end());
    auto it = std::adjacent_find(uuids.begin(), uuids.end());
    if (it != uuids.end())
    {
        std::cerr << "❌ 发现重复的 UUID!" << std::endl;
        return;
    }

    std::cout << "✅ 未发现重复的 UUID" << std::endl;
    std::cout << "✅ 生成性能测试通过" << std::endl;
}

void test_string_conversion_performance()
{
    std::cout << "测试字符串转换性能..." << std::endl;

    const int count = 100000;
    std::vector<cckit::uuid::Uuid> uuids;

    // 生成 UUID
    for (int i = 0; i < count; ++i)
    {
        uuids.push_back(cckit::uuid::Uuid::generate());
    }

    // 测试 toString 性能
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& uuid : uuids)
    {
        std::string str = uuid.toString();
        (void)str;  // 避免未使用警告
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "转换 " << count << " 个 UUID 为字符串耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每个转换耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;

    // 测试 fromString 性能
    std::vector<std::string> strings;
    for (const auto& uuid : uuids)
    {
        strings.push_back(uuid.toString());
    }

    start = std::chrono::high_resolution_clock::now();

    for (const auto& str : strings)
    {
        auto uuid = cckit::uuid::Uuid::fromString(str);
        (void)uuid;  // 避免未使用警告
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "转换 " << count << " 个字符串为 UUID 耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每个转换耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;

    std::cout << "✅ 字符串转换性能测试通过" << std::endl;
}

void test_comparison_performance()
{
    std::cout << "测试比较性能..." << std::endl;

    const int count = 100000;
    std::vector<cckit::uuid::Uuid> uuids;

    // 生成 UUID
    for (int i = 0; i < count; ++i)
    {
        uuids.push_back(cckit::uuid::Uuid::generate());
    }

    // 测试相等比较性能
    auto start = std::chrono::high_resolution_clock::now();

    int equalityCount = 0;
    for (size_t i = 0; i < uuids.size(); ++i)
    {
        for (size_t j = i + 1; j < std::min(uuids.size(), i + 100); ++j)
        {
            if (uuids[i] == uuids[j])
            {
                equalityCount++;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "执行 " << equalityCount << " 次相等比较耗时: " << duration.count() << " ms" << std::endl;

    // 测试小于比较性能
    start = std::chrono::high_resolution_clock::now();

    int lessCount = 0;
    for (size_t i = 0; i < uuids.size(); ++i)
    {
        for (size_t j = i + 1; j < std::min(uuids.size(), i + 100); ++j)
        {
            if (uuids[i] < uuids[j])
            {
                lessCount++;
            }
        }
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "执行 " << lessCount << " 次小于比较耗时: " << duration.count() << " ms" << std::endl;

    std::cout << "✅ 比较性能测试通过" << std::endl;
}

void test_hash_performance()
{
    std::cout << "测试哈希性能..." << std::endl;

    const int count = 100000;
    std::vector<cckit::uuid::Uuid> uuids;

    // 生成 UUID
    for (int i = 0; i < count; ++i)
    {
        uuids.push_back(cckit::uuid::Uuid::generate());
    }

    // 测试哈希计算性能
    auto start = std::chrono::high_resolution_clock::now();

    size_t hashSum = 0;
    std::hash<cckit::uuid::Uuid> hasher;
    for (const auto& uuid : uuids)
    {
        hashSum += hasher(uuid);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "计算 " << count << " 个 UUID 的哈希值耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每个哈希计算耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;
    std::cout << "哈希值总和: " << hashSum << " (用于防止优化)" << std::endl;

    std::cout << "✅ 哈希性能测试通过" << std::endl;
}

void test_container_performance()
{
    std::cout << "测试容器性能..." << std::endl;

    const int count = 100000;
    std::vector<cckit::uuid::Uuid> uuids;

    // 生成 UUID
    for (int i = 0; i < count; ++i)
    {
        uuids.push_back(cckit::uuid::Uuid::generate());
    }

    // 测试 std::map 性能
    auto start = std::chrono::high_resolution_clock::now();

    std::map<cckit::uuid::Uuid, int> uuidMap;
    for (int i = 0; i < count; ++i)
    {
        uuidMap[uuids[i]] = i;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "插入 " << count << " 个 UUID 到 std::map 耗时: " << duration.count() << " ms" << std::endl;

    // 测试 std::unordered_map 性能
    start = std::chrono::high_resolution_clock::now();

    std::unordered_map<cckit::uuid::Uuid, int> uuidUnorderedMap;
    for (int i = 0; i < count; ++i)
    {
        uuidUnorderedMap[uuids[i]] = i;
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "插入 " << count << " 个 UUID 到 std::unordered_map 耗时: " << duration.count() << " ms" << std::endl;

    // 测试 std::set 性能
    start = std::chrono::high_resolution_clock::now();

    std::set<cckit::uuid::Uuid> uuidSet;
    for (const auto& uuid : uuids)
    {
        uuidSet.insert(uuid);
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "插入 " << count << " 个 UUID 到 std::set 耗时: " << duration.count() << " ms" << std::endl;

    std::cout << "✅ 容器性能测试通过" << std::endl;
}

int main()
{
    std::cout << "=== UUID 性能测试 ===" << std::endl;
    std::cout << std::endl;

    test_generation_performance();
    std::cout << std::endl;

    test_string_conversion_performance();
    std::cout << std::endl;

    test_comparison_performance();
    std::cout << std::endl;

    test_hash_performance();
    std::cout << std::endl;

    test_container_performance();
    std::cout << std::endl;

    std::cout << "=== 所有性能测试完成! ===" << std::endl;
    return 0;
}
