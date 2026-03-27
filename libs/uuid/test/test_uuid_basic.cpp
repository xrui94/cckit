// test_uuid_basic.cpp - UUID 基础功能测试
#include <cckit/uuid/Uuid.hpp>

#include <iostream>
#include <cassert>
#include <set>
#include <unordered_map>
#include <string>

void test_nil_uuid()
{
    std::cout << "测试 nil UUID..." << std::endl;

    cckit::uuid::Uuid nilUuid;
    assert(nilUuid.isNil());
    assert(nilUuid.toString() == "00000000-0000-0000-0000-000000000000");

    std::cout << "✅ nil UUID 测试通过" << std::endl;
}

void test_generate_uuid()
{
    std::cout << "测试 UUID 生成..." << std::endl;

    // 生成多个 UUID，确保它们不重复
    std::set<cckit::uuid::Uuid> uuids;
    for (int i = 0; i < 100; ++i)
    {
        cckit::uuid::Uuid uuid = cckit::uuid::Uuid::generate();
        assert(!uuid.isNil());
        assert(uuids.insert(uuid).second); // 插入成功表示没有重复
    }

    std::cout << "✅ UUID 生成测试通过" << std::endl;
}

void test_string_conversion()
{
    std::cout << "测试字符串转换..." << std::endl;

    // 测试有效的 UUID 字符串
    std::string validStr = "550e8400-e29b-41d4-a716-446655440000";
    auto uuidOpt = cckit::uuid::Uuid::fromString(validStr);
    assert(uuidOpt.has_value());
    assert(uuidOpt->toString() == validStr);

    // 测试无效的 UUID 字符串
    std::string invalidStr = "invalid-uuid-string";
    auto invalidOpt = cckit::uuid::Uuid::fromString(invalidStr);
    assert(!invalidOpt.has_value());

    // 测试大小写不敏感
    std::string lowerCaseStr = "550e8400-e29b-41d4-a716-446655440000";
    std::string upperCaseStr = "550E8400-E29B-41D4-A716-446655440000";
    auto lowerUuid = cckit::uuid::Uuid::fromString(lowerCaseStr);
    auto upperUuid = cckit::uuid::Uuid::fromString(upperCaseStr);
    assert(lowerUuid.has_value());
    assert(upperUuid.has_value());
    assert(*lowerUuid == *upperUuid);

    // 测试往返转换
    cckit::uuid::Uuid generated = cckit::uuid::Uuid::generate();
    std::string str1 = generated.toString();
    auto parsed = cckit::uuid::Uuid::fromString(str1);
    assert(parsed.has_value());
    std::string str2 = parsed->toString();
    assert(str1 == str2);

    std::cout << "✅ 字符串转换测试通过" << std::endl;
}

void test_comparison_operators()
{
    std::cout << "测试比较运算符..." << std::endl;

    cckit::uuid::Uuid uuid1 = cckit::uuid::Uuid::generate();
    cckit::uuid::Uuid uuid2 = cckit::uuid::Uuid::generate();
    cckit::uuid::Uuid uuid3 = uuid1;

    // 测试相等
    assert(uuid1 == uuid3);
    assert(!(uuid1 == uuid2));

    // 测试不等
    assert(uuid1 != uuid2);
    assert(!(uuid1 != uuid3));

    // 测试小于运算符（字典序）
    std::string str1 = "00000000-0000-0000-0000-000000000001";
    std::string str2 = "00000000-0000-0000-0000-000000000002";
    auto u1 = cckit::uuid::Uuid::fromString(str1);
    auto u2 = cckit::uuid::Uuid::fromString(str2);
    assert(u1.has_value());
    assert(u2.has_value());
    assert(*u1 < *u2);
    assert(!(*u2 < *u1));

    std::cout << "✅ 比较运算符测试通过" << std::endl;
}

void test_hash_function()
{
    std::cout << "测试哈希函数..." << std::endl;

    // 测试 std::unordered_map
    std::unordered_map<cckit::uuid::Uuid, std::string> uuidMap;

    cckit::uuid::Uuid uuid1 = cckit::uuid::Uuid::generate();
    cckit::uuid::Uuid uuid2 = cckit::uuid::Uuid::generate();

    uuidMap[uuid1] = "first";
    uuidMap[uuid2] = "second";

    assert(uuidMap[uuid1] == "first");
    assert(uuidMap[uuid2] == "second");
    assert(uuidMap.size() == 2);

    // 测试相同的 UUID 产生相同的哈希值
    cckit::uuid::Uuid uuid3 = uuid1;
    assert(std::hash<cckit::uuid::Uuid>{}(uuid1) == std::hash<cckit::uuid::Uuid>{}(uuid3));

    std::cout << "✅ 哈希函数测试通过" << std::endl;
}

void test_pod_conversion()
{
    std::cout << "测试 POD 转换..." << std::endl;

    cckit::uuid::Uuid uuid = cckit::uuid::Uuid::generate();

    // 测试隐式转换到 POD
    const cckit_uuid_t& pod = uuid;
    assert(!cckit_uuid_is_nil(&pod));

    // 测试获取底层 POD
    const cckit_uuid_t& data = uuid.data();
    assert(!cckit_uuid_is_nil(&data));

    // 测试从 POD 构造
    cckit::uuid::Uuid uuid2(data);
    assert(uuid == uuid2);

    std::cout << "✅ POD 转换测试通过" << std::endl;
}

void test_convenience_function()
{
    std::cout << "测试便捷函数..." << std::endl;

    cckit::uuid::Uuid uuid = cckit::uuid::genUuid();
    assert(!uuid.isNil());
    assert(uuid.toString().length() == 36);

    std::cout << "✅ 便捷函数测试通过" << std::endl;
}

void test_copy_and_assignment()
{
    std::cout << "测试拷贝和赋值..." << std::endl;

    cckit::uuid::Uuid uuid1 = cckit::uuid::Uuid::generate();
    cckit::uuid::Uuid uuid2(uuid1);  // 拷贝构造
    cckit::uuid::Uuid uuid3;
    uuid3 = uuid1;  // 赋值

    assert(uuid1 == uuid2);
    assert(uuid1 == uuid3);
    assert(uuid1.toString() == uuid2.toString());
    assert(uuid1.toString() == uuid3.toString());

    std::cout << "✅ 拷贝和赋值测试通过" << std::endl;
}

void test_edge_cases()
{
    std::cout << "测试边界情况..." << std::endl;

    // 测试空字符串
    auto emptyOpt = cckit::uuid::Uuid::fromString("");
    assert(!emptyOpt.has_value());

    // 测试过长的字符串
    auto longOpt = cckit::uuid::Uuid::fromString("550e8400-e29b-41d4-a716-446655440000-extra");
    assert(!longOpt.has_value());

    // 测试过短的字符串
    auto shortOpt = cckit::uuid::Uuid::fromString("550e8400-e29b");
    assert(!shortOpt.has_value());

    // 测试格式错误的字符串（缺少连字符）（目前是支持没有连接字符的uuid字符串的）
    //auto noDashOpt = cckit::uuid::Uuid::fromString("550e8400e29b41d4a716446655440000");
    //assert(!noDashOpt.has_value());

    // 测试无连字符格式（应该支持）
    auto noDashOpt = cckit::uuid::Uuid::fromString("550e8400e29b41d4a716446655440000");
    assert(noDashOpt.has_value());  // ✅ 支持无连字符格式

    // 测试带花括号格式（应该支持）
    auto braceOpt = cckit::uuid::Uuid::fromString("{550e8400-e29b-41d4-a716-446655440000}");
    assert(braceOpt.has_value());  // ✅ 支持带花括号格式

    // 验证不同格式解析结果相同
    auto standardOpt = cckit::uuid::Uuid::fromString("550e8400-e29b-41d4-a716-446655440000");
    assert(standardOpt.has_value());
    assert(noDashOpt.value() == standardOpt.value());
    assert(braceOpt.value() == standardOpt.value());

    std::cout << "✅ 边界情况测试通过" << std::endl;
}

int main()
{
    std::cout << "=== UUID 基础功能测试 ===" << std::endl;
    std::cout << std::endl;

    test_nil_uuid();
    std::cout << std::endl;

    test_generate_uuid();
    std::cout << std::endl;

    test_string_conversion();
    std::cout << std::endl;

    test_comparison_operators();
    std::cout << std::endl;

    test_hash_function();
    std::cout << std::endl;

    test_pod_conversion();
    std::cout << std::endl;

    test_convenience_function();
    std::cout << std::endl;

    test_copy_and_assignment();
    std::cout << std::endl;

    test_edge_cases();
    std::cout << std::endl;

    std::cout << "=== 所有测试通过! ===" << std::endl;
    return 0;
}
