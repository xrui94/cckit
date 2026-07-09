// test_string_performance.cpp - 字符串工具性能测试
#include <cckit/string/String.hpp>

#include <gtest/gtest.h>
#include <chrono>
#include <vector>

TEST(StringPerformance, Trim)
{
    const int count = 100000;
    std::vector<std::string> testStrings;
    for (int i = 0; i < count; ++i)
        testStrings.push_back("  " + std::string(i % 100, 'a') + "  ");

    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& str : testStrings) {
        auto result = cckit::string::trim(str);
        (void)result;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "修剪 " << count << " 个字符串耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次修剪耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;
    SUCCEED();
}

TEST(StringPerformance, CaseConversion)
{
    const int count = 100000;
    std::vector<std::string> testStrings;
    for (int i = 0; i < count; ++i)
        testStrings.push_back("Hello World " + std::to_string(i));

    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& str : testStrings) {
        auto result = cckit::string::toLower(str);
        (void)result;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "转换 " << count << " 个字符串为小写耗时: " << duration.count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (const auto& str : testStrings) {
        auto result = cckit::string::toUpper(str);
        (void)result;
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "转换 " << count << " 个字符串为大写耗时: " << duration.count() << " ms" << std::endl;
    SUCCEED();
}

TEST(StringPerformance, Split)
{
    const int count = 10000;
    std::vector<std::string> testStrings;
    for (int i = 0; i < count; ++i) {
        std::string str;
        for (int j = 0; j < 10; ++j)
            str += "part" + std::to_string(j) + ",";
        testStrings.push_back(str);
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& str : testStrings) {
        auto parts = cckit::string::split(str, ",");
        (void)parts;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "分割 " << count << " 个字符串耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次分割耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;
    SUCCEED();
}

TEST(StringPerformance, StartsWith)
{
    const int count = 1000000;
    std::vector<std::string> testStrings;
    for (int i = 0; i < count; ++i)
        testStrings.push_back("Hello World " + std::to_string(i));

    auto start = std::chrono::high_resolution_clock::now();
    int matchCount = 0;
    for (const auto& str : testStrings) {
        if (cckit::string::startsWith(str, "Hello"))
            matchCount++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "检查 " << count << " 个字符串前缀耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "匹配数: " << matchCount << std::endl;
    std::cout << "平均每次检查耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;
    SUCCEED();
}

TEST(StringPerformance, EndsWith)
{
    const int count = 1000000;
    std::vector<std::string> testStrings;
    for (int i = 0; i < count; ++i)
        testStrings.push_back("Hello World " + std::to_string(i));

    auto start = std::chrono::high_resolution_clock::now();
    int matchCount = 0;
    for (const auto& str : testStrings) {
        if (cckit::string::endsWith(str, std::to_string(matchCount)))
            matchCount++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "检查 " << count << " 个字符串后缀耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "匹配数: " << matchCount << std::endl;
    SUCCEED();
}

TEST(StringPerformance, Utf8Validation)
{
    const int count = 100000;
    std::vector<std::string> testStrings;
    for (int i = 0; i < count; ++i)
        testStrings.push_back("Hello 你好 こんにちは 안녕하세요 " + std::to_string(i));

    auto start = std::chrono::high_resolution_clock::now();
    int validCount = 0;
    for (const auto& str : testStrings) {
        if (cckit::string::isValidUtf8(str))
            validCount++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "验证 " << count << " 个 UTF-8 字符串耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "有效字符串数: " << validCount << std::endl;
    std::cout << "平均每次验证耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;
    SUCCEED();
}

TEST(StringPerformance, CombinedOperations)
{
    const int count = 10000;
    std::vector<std::string> testStrings;
    for (int i = 0; i < count; ++i) {
        std::string str = "  " + std::string(i % 50, 'A') + ", " +
                         std::string(i % 30, 'B') + ", " +
                         std::string(i % 40, 'C') + "  ";
        testStrings.push_back(str);
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& str : testStrings) {
        auto trimmed = cckit::string::trim(str);
        auto parts = cckit::string::split(trimmed, ",");
        for (auto& part : parts) {
            auto lower = cckit::string::toLower(part);
            (void)lower;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "执行 " << count << " 次组合操作耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次组合操作耗时: " << (duration.count() * 1000.0 / count) << " ms" << std::endl;
    SUCCEED();
}
