// uuid.cpp - UUID 后端实现
// 
// 使用 Boost.UUID 作为后端

#include "cckit/uuid/Uuid.hpp"
#include "cckit/export.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/assert/source_location.hpp>

#include <cstring>
#include <stdexcept>

// 提供 boost::throw_exception 的实现
// 当使用 BOOST_NO_EXCEPTIONS 时，用户需要提供此函数
namespace boost
{
    void throw_exception(std::exception const& e)
    {
        throw e;
    }
    
    void throw_exception(std::exception const& e, boost::source_location const&)
    {
        throw e;
    }
}

namespace
{
    // 辅助函数：从 cckit_uuid_t 转换为 boost::uuids::uuid
    inline boost::uuids::uuid to_boost(const cckit_uuid_t* uuid)
    {
        boost::uuids::uuid result;
        std::memcpy(&result, uuid->data, 16);
        return result;
    }
    
    // 辅助函数：从 boost::uuids::uuid 转换为 cckit_uuid_t
    inline void from_boost(cckit_uuid_t* out, const boost::uuids::uuid& uuid)
    {
        std::memcpy(out->data, uuid.data, 16);
    }
}

extern "C"
{

    void cckit_uuid_generate(cckit_uuid_t* out)
    {
        static boost::uuids::random_generator gen;
        boost::uuids::uuid uuid = gen();
        from_boost(out, uuid);
    }

    bool cckit_uuid_is_nil(const cckit_uuid_t* uuid)
    {
        return to_boost(uuid).is_nil();
    }

    bool cckit_uuid_equal(const cckit_uuid_t* a, const cckit_uuid_t* b)
    {
        return to_boost(a) == to_boost(b);
    }

    void cckit_uuid_to_string(const cckit_uuid_t* uuid, char* out, size_t out_size)
    {
        boost::uuids::uuid boost_uuid = to_boost(uuid);
        std::string str = boost::uuids::to_string(boost_uuid);
        
        // 安全拷贝
        size_t copy_len = std::min(str.size(), out_size - 1);
        std::memcpy(out, str.c_str(), copy_len);
        out[copy_len] = '\0';
    }

    bool cckit_uuid_from_string(cckit_uuid_t* out, const char* str)
    {
        if (!out || !str) {
            return false;
        }

        size_t len = strlen(str);
        if (len == 0) {
            return false;
        }

        //// 更严格的验证：
        //// 验证字符串格式：必须是标准 UUID 格式 (8-4-4-4-12)
        //// 例如: 550e8400-e29b-41d4-a716-446655440000
        //if (len != 36) {
        //    return false;  // 长度必须为 36
        //}

        //// 验证连字符位置
        //if (str[8] != '-' || str[13] != '-' || str[18] != '-' || str[23] != '-') {
        //    return false;  // 连字符位置必须正确
        //}

        //// 验证其他位置必须是十六进制字符
        //for (size_t i = 0; i < 36; ++i) {
        //    if (i == 8 || i == 13 || i == 18 || i == 23) {
        //        continue;  // 跳过连字符位置
        //    }
        //    char c = str[i];
        //    if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) {
        //        return false;  // 必须是十六进制字符
        //    }
        //}

        try
        {
            boost::uuids::string_generator gen;
            boost::uuids::uuid uuid = gen(str);
            from_boost(out, uuid);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    size_t cckit_uuid_hash(const cckit_uuid_t* uuid)
    {
        // 使用 Boost.UUID 的哈希
        boost::uuids::uuid boost_uuid = to_boost(uuid);
        return std::hash<boost::uuids::uuid>{}(boost_uuid);
    }

} // extern "C"
