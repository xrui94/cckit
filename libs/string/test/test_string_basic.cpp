// test_string_basic.cpp - 字符串工具基础功能测试
#include <cckit/string/String.hpp>

#include <gtest/gtest.h>
#include <algorithm>

TEST(StringTest, StartsWith)
{
    EXPECT_TRUE(cckit::string::startsWith("Hello World", "Hello"));
    EXPECT_FALSE(cckit::string::startsWith("Hello World", "World"));
    EXPECT_TRUE(cckit::string::startsWith("Hello World", ""));

    EXPECT_TRUE(cckit::string::startsWith("Hello World", "hello", true));
    EXPECT_TRUE(cckit::string::startsWith("HELLO WORLD", "hello", true));
    EXPECT_FALSE(cckit::string::startsWith("Hello World", "hello", false));
}

TEST(StringTest, EndsWith)
{
    EXPECT_TRUE(cckit::string::endsWith("Hello World", "World"));
    EXPECT_FALSE(cckit::string::endsWith("Hello World", "Hello"));
    EXPECT_TRUE(cckit::string::endsWith("Hello World", ""));

    EXPECT_TRUE(cckit::string::endsWith("Hello World", "world", true));
    EXPECT_TRUE(cckit::string::endsWith("HELLO WORLD", "world", true));
    EXPECT_FALSE(cckit::string::endsWith("Hello World", "world", false));
}

TEST(StringTest, Trim)
{
    EXPECT_EQ(cckit::string::trim("  Hello  "), "Hello");
    EXPECT_EQ(cckit::string::trim("\tHello\t"), "Hello");
    EXPECT_EQ(cckit::string::trim("\nHello\n"), "Hello");

    EXPECT_EQ(cckit::string::trim("*Hello*", "*"), "Hello");
    EXPECT_EQ(cckit::string::trim("---Hello---", "-"), "Hello");

    EXPECT_EQ(cckit::string::trim("H e l l o", " ", true), "Hello");
    EXPECT_EQ(cckit::string::trim("H-e-l-l-o", "-", true), "Hello");

    EXPECT_EQ(cckit::string::trim(""), "");
    EXPECT_EQ(cckit::string::trim("   "), "");
}

TEST(StringTest, CaseConversion)
{
    EXPECT_EQ(cckit::string::toLower("HELLO WORLD"), "hello world");
    EXPECT_EQ(cckit::string::toUpper("hello world"), "HELLO WORLD");
    EXPECT_EQ(cckit::string::toLower("Hello World"), "hello world");
    EXPECT_EQ(cckit::string::toUpper("Hello World"), "HELLO WORLD");

    EXPECT_EQ(cckit::string::toLower(""), "");
    EXPECT_EQ(cckit::string::toUpper(""), "");

    EXPECT_EQ(cckit::string::toLower("123 ABC!@#"), "123 abc!@#");
    EXPECT_EQ(cckit::string::toUpper("123 abc!@#"), "123 ABC!@#");
}

TEST(StringTest, Equals)
{
    EXPECT_TRUE(cckit::string::equals("Hello", "Hello"));
    EXPECT_FALSE(cckit::string::equals("Hello", "hello"));

    EXPECT_TRUE(cckit::string::equals("Hello", "hello", true));
    EXPECT_TRUE(cckit::string::equals("HELLO", "hello", true));
    EXPECT_TRUE(cckit::string::equals("HeLLo", "hElLo", true));

    EXPECT_TRUE(cckit::string::equals("", ""));
    EXPECT_TRUE(cckit::string::equals("", "", true));
}

TEST(StringTest, Split)
{
    auto parts1 = cckit::string::split("a,b,c", ",");
    ASSERT_EQ(parts1.size(), 3);
    EXPECT_EQ(parts1[0], "a");
    EXPECT_EQ(parts1[1], "b");
    EXPECT_EQ(parts1[2], "c");

    auto parts2 = cckit::string::split("a::b::c", "::");
    ASSERT_EQ(parts2.size(), 3);

    auto parts3 = cckit::string::split("a,,c", ",");
    ASSERT_EQ(parts3.size(), 3);
    EXPECT_EQ(parts3[0], "a");
    EXPECT_EQ(parts3[1], "");
    EXPECT_EQ(parts3[2], "c");

    auto parts3b = cckit::string::split("a,b,", ",");
    ASSERT_EQ(parts3b.size(), 2);
    EXPECT_EQ(parts3b[0], "a");
    EXPECT_EQ(parts3b[1], "b");

    auto parts4 = cckit::string::split("a,,c", ",", cckit::string::SplitMode::KeepAll);
    ASSERT_EQ(parts4.size(), 3);

    auto parts5 = cckit::string::split("a,,c", ",", cckit::string::SplitMode::SkipAll);
    ASSERT_EQ(parts5.size(), 2);

    auto parts6 = cckit::string::split("", ",");
    EXPECT_TRUE(parts6.empty());
}

TEST(StringTest, Utf8Validation)
{
    EXPECT_TRUE(cckit::string::isValidUtf8("Hello"));
    EXPECT_TRUE(cckit::string::isValidUtf8("你好"));
    EXPECT_TRUE(cckit::string::isValidUtf8("こんにちは"));
    EXPECT_TRUE(cckit::string::isValidUtf8("안녕하세요"));
    EXPECT_TRUE(cckit::string::isValidUtf8(""));

    std::string invalidUtf8 = "Hello\xFF\xFE";
    EXPECT_FALSE(cckit::string::isValidUtf8(invalidUtf8));
}

TEST(StringTest, CaseConversionEdgeCases)
{
    std::string mixed = "ABC123def!@#你好";
    EXPECT_EQ(cckit::string::toLower(mixed), "abc123def!@#你好");
    EXPECT_EQ(cckit::string::toUpper(mixed), "ABC123DEF!@#你好");

    std::string special = "a\tb\nc d";
    EXPECT_EQ(cckit::string::toLower(special), "a\tb\nc d");
    EXPECT_EQ(cckit::string::toUpper(special), "A\tB\nC D");
}

TEST(StringTest, SplitEdgeCases)
{
    auto parts1 = cckit::string::split("a,,,b", ",");
    ASSERT_EQ(parts1.size(), 4);
    EXPECT_EQ(parts1[0], "a");
    EXPECT_EQ(parts1[1], "");
    EXPECT_EQ(parts1[2], "");
    EXPECT_EQ(parts1[3], "b");

    auto parts2 = cckit::string::split(",a,b,", ",");
    ASSERT_EQ(parts2.size(), 3);
    EXPECT_EQ(parts2[0], "");
    EXPECT_EQ(parts2[1], "a");
    EXPECT_EQ(parts2[2], "b");

    auto parts3 = cckit::string::split(",,,", ",");
    ASSERT_EQ(parts3.size(), 3);
    EXPECT_EQ(parts3[0], "");
    EXPECT_EQ(parts3[1], "");
    EXPECT_EQ(parts3[2], "");

    auto parts4 = cckit::string::split("abc", "");
    ASSERT_EQ(parts4.size(), 1);
    EXPECT_EQ(parts4[0], "abc");

    auto parts5 = cckit::string::split("a,你,b,好", ",");
    ASSERT_EQ(parts5.size(), 4);
    EXPECT_EQ(parts5[1], "你");
    EXPECT_EQ(parts5[3], "好");
}

TEST(StringTest, TrimEdgeCases)
{
    EXPECT_EQ(cckit::string::trim(" \t\n\r Hello \t\n\r "), "Hello");
    EXPECT_EQ(cckit::string::trim("   "), "");
    EXPECT_EQ(cckit::string::trim("\t\n\r"), "");
    EXPECT_EQ(cckit::string::trim("Hello"), "Hello");
    EXPECT_EQ(cckit::string::trim("H"), "H");
    EXPECT_EQ(cckit::string::trim("H  e  l  l  o", " ", true), "Hello");
}

TEST(StringTest, Combinations)
{
    std::string input = "  apple, banana , cherry  ";
    auto trimmed = cckit::string::trim(input);
    auto parts = cckit::string::split(trimmed, ",");
    ASSERT_EQ(parts.size(), 3);
    EXPECT_EQ(cckit::string::trim(parts[0]), "apple");
    EXPECT_EQ(cckit::string::trim(parts[1]), "banana");
    EXPECT_EQ(cckit::string::trim(parts[2]), "cherry");

    std::string str = "  #include <iostream>";
    EXPECT_TRUE(cckit::string::startsWith(cckit::string::trim(str), "#"));

    EXPECT_TRUE(cckit::string::equals(
        cckit::string::toLower("HELLO"),
        cckit::string::toLower("hello"),
        false
    ));
}
