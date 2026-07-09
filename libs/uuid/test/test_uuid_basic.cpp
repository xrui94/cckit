// test_uuid_basic.cpp - UUID 基础功能测试
#include <cckit/uuid/Uuid.hpp>

#include <gtest/gtest.h>
#include <set>
#include <unordered_map>

TEST(UuidTest, NilUuid)
{
    cckit::uuid::Uuid nilUuid;
    EXPECT_TRUE(nilUuid.isNil());
    EXPECT_EQ(nilUuid.toString(), "00000000-0000-0000-0000-000000000000");
}

TEST(UuidTest, GenerateUuid)
{
    std::set<cckit::uuid::Uuid> uuids;
    for (int i = 0; i < 100; ++i) {
        cckit::uuid::Uuid uuid = cckit::uuid::Uuid::generate();
        EXPECT_FALSE(uuid.isNil());
        EXPECT_TRUE(uuids.insert(uuid).second);
    }
}

TEST(UuidTest, StringConversion)
{
    std::string validStr = "550e8400-e29b-41d4-a716-446655440000";
    auto uuidOpt = cckit::uuid::Uuid::fromString(validStr);
    ASSERT_TRUE(uuidOpt.has_value());
    EXPECT_EQ(uuidOpt->toString(), validStr);

    std::string invalidStr = "invalid-uuid-string";
    auto invalidOpt = cckit::uuid::Uuid::fromString(invalidStr);
    EXPECT_FALSE(invalidOpt.has_value());

    std::string emptyStr = "";
    auto emptyOpt = cckit::uuid::Uuid::fromString(emptyStr);
    EXPECT_FALSE(emptyOpt.has_value());

    std::string partialStr = "550e8400-e29b-41d4-a716";
    auto partialOpt = cckit::uuid::Uuid::fromString(partialStr);
    EXPECT_FALSE(partialOpt.has_value());
}

TEST(UuidTest, Comparison)
{
    cckit::uuid::Uuid u1 = cckit::uuid::Uuid::generate();
    cckit::uuid::Uuid u2 = cckit::uuid::Uuid::generate();
    cckit::uuid::Uuid u3 = u1;

    EXPECT_TRUE(u1 == u3);
    EXPECT_FALSE(u1 == u2);
    EXPECT_TRUE(u1 != u2);
    EXPECT_FALSE(u1 != u3);

    cckit::uuid::Uuid nilUuid;
    EXPECT_TRUE(nilUuid == nilUuid);
    EXPECT_FALSE(nilUuid == u1);
}

TEST(UuidTest, Hashing)
{
    std::unordered_map<cckit::uuid::Uuid, int> map;
    cckit::uuid::Uuid u1 = cckit::uuid::Uuid::generate();
    cckit::uuid::Uuid u2 = cckit::uuid::Uuid::generate();

    map[u1] = 1;
    map[u2] = 2;

    EXPECT_EQ(map[u1], 1);
    EXPECT_EQ(map[u2], 2);
    EXPECT_EQ(map.size(), 2);

    cckit::uuid::Uuid nilUuid;
    map[nilUuid] = 0;
    EXPECT_EQ(map[nilUuid], 0);
    EXPECT_EQ(map.size(), 3);
}

TEST(UuidTest, CopyAndMove)
{
    cckit::uuid::Uuid u1 = cckit::uuid::Uuid::generate();
    cckit::uuid::Uuid u2 = u1;
    EXPECT_EQ(u1, u2);
    EXPECT_EQ(u1.toString(), u2.toString());

    cckit::uuid::Uuid u3 = std::move(u2);
    EXPECT_EQ(u1, u3);
}

TEST(UuidTest, AllUnique)
{
    const int count = 1000;
    std::set<std::string> strUuids;
    for (int i = 0; i < count; ++i) {
        cckit::uuid::Uuid uuid = cckit::uuid::Uuid::generate();
        EXPECT_TRUE(strUuids.insert(uuid.toString()).second);
    }
    EXPECT_EQ(strUuids.size(), count);
}
