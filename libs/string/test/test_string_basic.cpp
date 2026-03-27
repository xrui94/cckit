// test_string_basic.cpp - 字符串工具基础功能测试
#include <cckit/string/String.hpp>

#include <iostream>
#include <cassert>
#include <algorithm>

void test_starts_with()
{
    std::cout << "测试 startsWith..." << std::endl;

    assert(cckit::string::startsWith("Hello World", "Hello"));
    assert(!cckit::string::startsWith("Hello World", "World"));
    assert(cckit::string::startsWith("Hello World", ""));

    // 大小写不敏感测试
    assert(cckit::string::startsWith("Hello World", "hello", true));
    assert(cckit::string::startsWith("HELLO WORLD", "hello", true));
    assert(!cckit::string::startsWith("Hello World", "hello", false));

    std::cout << "✅ startsWith 测试通过" << std::endl;
}

void test_ends_with()
{
    std::cout << "测试 endsWith..." << std::endl;

    assert(cckit::string::endsWith("Hello World", "World"));
    assert(!cckit::string::endsWith("Hello World", "Hello"));
    assert(cckit::string::endsWith("Hello World", ""));

    // 大小写不敏感测试
    assert(cckit::string::endsWith("Hello World", "world", true));
    assert(cckit::string::endsWith("HELLO WORLD", "world", true));
    assert(!cckit::string::endsWith("Hello World", "world", false));

    std::cout << "✅ endsWith 测试通过" << std::endl;
}

void test_trim()
{
    std::cout << "测试 trim..." << std::endl;

    // 基本修剪
    assert(cckit::string::trim("  Hello  ") == "Hello");
    assert(cckit::string::trim("\tHello\t") == "Hello");
    assert(cckit::string::trim("\nHello\n") == "Hello");

    // 自定义字符
    assert(cckit::string::trim("*Hello*", "*") == "Hello");
    assert(cckit::string::trim("---Hello---", "-") == "Hello");

    // 移除中间字符
    assert(cckit::string::trim("H e l l o", " ", true) == "Hello");
    assert(cckit::string::trim("H-e-l-l-o", "-", true) == "Hello");

    // 空字符串
    assert(cckit::string::trim("") == "");
    assert(cckit::string::trim("   ") == "");

    std::cout << "✅ trim 测试通过" << std::endl;
}

void test_case_conversion()
{
    std::cout << "测试大小写转换..." << std::endl;

    assert(cckit::string::toLower("HELLO WORLD") == "hello world");
    assert(cckit::string::toUpper("hello world") == "HELLO WORLD");
    assert(cckit::string::toLower("Hello World") == "hello world");
    assert(cckit::string::toUpper("Hello World") == "HELLO WORLD");

    // 空字符串
    assert(cckit::string::toLower("") == "");
    assert(cckit::string::toUpper("") == "");

    // 数字和符号
    assert(cckit::string::toLower("123 ABC!@#") == "123 abc!@#");
    assert(cckit::string::toUpper("123 abc!@#") == "123 ABC!@#");

    std::cout << "✅ 大小写转换测试通过" << std::endl;
}

void test_equals()
{
    std::cout << "测试字符串比较..." << std::endl;

    // 大小写敏感
    assert(cckit::string::equals("Hello", "Hello"));
    assert(!cckit::string::equals("Hello", "hello"));

    // 大小写不敏感
    assert(cckit::string::equals("Hello", "hello", true));
    assert(cckit::string::equals("HELLO", "hello", true));
    assert(cckit::string::equals("HeLLo", "hElLo", true));

    // 空字符串
    assert(cckit::string::equals("", ""));
    assert(cckit::string::equals("", "", true));

    std::cout << "✅ 字符串比较测试通过" << std::endl;
}

void test_split()
{
    std::cout << "测试字符串分割..." << std::endl;

    // 基本分割
    auto parts1 = cckit::string::split("a,b,c", ",");
    assert(parts1.size() == 3);
    assert(parts1[0] == "a" && parts1[1] == "b" && parts1[2] == "c");

    // 多字符分隔符
    auto parts2 = cckit::string::split("a::b::c", "::");
    assert(parts2.size() == 3);

    // 含空字符串的分割
    auto parts3 = cckit::string::split("a,,c", ",");
    assert(parts3.size() == 3); // TrimTrailing 模式保留中间空字符串
    assert(parts3[0] == "a" && parts3[1] == "" && parts3[2] == "c");

    // TrimTrailing 模式：丢弃尾随空字符串
    auto parts3b = cckit::string::split("a,b,", ",");
    assert(parts3b.size() == 2); // 尾随空字符串被丢弃
    assert(parts3b[0] == "a" && parts3b[1] == "b");

    // KeepAll 模式
    auto parts4 = cckit::string::split("a,,c", ",", cckit::string::SplitMode::KeepAll);
    assert(parts4.size() == 3);

    // SkipAll 模式
    auto parts5 = cckit::string::split("a,,c", ",", cckit::string::SplitMode::SkipAll);
    assert(parts5.size() == 2);

    // 空字符串
    auto parts6 = cckit::string::split("", ",");
    assert(parts6.empty());

    std::cout << "✅ 字符串分割测试通过" << std::endl;
}

void test_utf8_validation()
{
    std::cout << "测试 UTF-8 验证..." << std::endl;

    // 有效的 UTF-8
    assert(cckit::string::isValidUtf8("Hello"));
    assert(cckit::string::isValidUtf8("你好"));
    assert(cckit::string::isValidUtf8("こんにちは"));
    assert(cckit::string::isValidUtf8("안녕하세요"));

    // 空字符串
    assert(cckit::string::isValidUtf8(""));

    // 无效的 UTF-8（包含不完整的字节序列）
    std::string invalidUtf8 = "Hello\xFF\xFE";
    assert(!cckit::string::isValidUtf8(invalidUtf8));

    std::cout << "✅ UTF-8 验证测试通过" << std::endl;
}

void test_case_conversion_edge_cases()
{
    std::cout << "测试大小写转换边界情况..." << std::endl;

    // 混合内容
    std::string mixed = "ABC123def!@#你好";
    assert(cckit::string::toLower(mixed) == "abc123def!@#你好");
    assert(cckit::string::toUpper(mixed) == "ABC123DEF!@#你好");

    // 特殊字符
    std::string special = "a\tb\nc d";
    assert(cckit::string::toLower(special) == "a\tb\nc d");
    assert(cckit::string::toUpper(special) == "A\tB\nC D");

    std::cout << "✅ 大小写转换边界情况测试通过" << std::endl;
}

void test_split_edge_cases()
{
    std::cout << "测试字符串分割边界情况..." << std::endl;

    // 连续分隔符
    auto parts1 = cckit::string::split("a,,,b", ",");
    assert(parts1.size() == 4);  // TrimTrailing: ["a", "", "", "b"] - 保留中间空
    assert(parts1[0] == "a" && parts1[1] == "" && parts1[2] == "" && parts1[3] == "b");

    // 首尾分隔符
    auto parts2 = cckit::string::split(",a,b,", ",");
    assert(parts2.size() == 3);  // TrimTrailing: ["", "a", "b"] - 前导空保留，尾随空丢弃
    assert(parts2[0] == "" && parts2[1] == "a" && parts2[2] == "b");

    // 只有分隔符
    auto parts3 = cckit::string::split(",,,", ",");
    assert(parts3.size() == 3);  // TrimTrailing: ["", "", ""] - 前导/中间空保留，尾随空丢弃
    assert(parts3[0] == "" && parts3[1] == "" && parts3[2] == "");

    // 空分隔符
    auto parts4 = cckit::string::split("abc", "");
    assert(parts4.size() == 1);
    assert(parts4[0] == "abc");

    // UTF-8 分割
    auto parts5 = cckit::string::split("a,你,b,好", ",");
    assert(parts5.size() == 4);
    assert(parts5[1] == "你");
    assert(parts5[3] == "好");

    std::cout << "✅ 字符串分割边界情况测试通过" << std::endl;
}

void test_trim_edge_cases()
{
    std::cout << "测试 trim 边界情况..." << std::endl;

    // 多种空白字符
    assert(cckit::string::trim(" \t\n\r Hello \t\n\r ") == "Hello");

    // 只有空白字符
    assert(cckit::string::trim("   ") == "");
    assert(cckit::string::trim("\t\n\r") == "");

    // 已经修剪过的字符串
    assert(cckit::string::trim("Hello") == "Hello");

    // 单个字符
    assert(cckit::string::trim("H") == "H");

    // 中间字符移除
    assert(cckit::string::trim("H  e  l  l  o", " ", true) == "Hello");

    std::cout << "✅ trim 边界情况测试通过" << std::endl;
}

void test_string_combinations()
{
    std::cout << "测试字符串组合操作..." << std::endl;

    // 组合使用 trim 和 split
    std::string input = "  apple, banana , cherry  ";
    auto trimmed = cckit::string::trim(input);
    auto parts = cckit::string::split(trimmed, ",");

    assert(parts.size() == 3);
    assert(cckit::string::trim(parts[0]) == "apple");
    assert(cckit::string::trim(parts[1]) == "banana");
    assert(cckit::string::trim(parts[2]) == "cherry");

    // 组合使用 startsWith 和 trim
    std::string str = "  #include <iostream>";
    assert(cckit::string::startsWith(cckit::string::trim(str), "#"));

    // 组合使用 toLower 和 equals
    assert(cckit::string::equals(
        cckit::string::toLower("HELLO"),
        cckit::string::toLower("hello"),
        false
    ));

    std::cout << "✅ 字符串组合操作测试通过" << std::endl;
}

int main()
{
    std::cout << "=== 字符串工具基础功能测试 ===" << std::endl;
    std::cout << std::endl;

    test_starts_with();
    std::cout << std::endl;

    test_ends_with();
    std::cout << std::endl;

    test_trim();
    std::cout << std::endl;

    test_case_conversion();
    std::cout << std::endl;

    test_equals();
    std::cout << std::endl;

    test_split();
    std::cout << std::endl;

    test_utf8_validation();
    std::cout << std::endl;

    test_case_conversion_edge_cases();
    std::cout << std::endl;

    test_split_edge_cases();
    std::cout << std::endl;

    test_trim_edge_cases();
    std::cout << std::endl;

    test_string_combinations();
    std::cout << std::endl;

    std::cout << "=== 所有测试通过! ===" << std::endl;
    return 0;
}
