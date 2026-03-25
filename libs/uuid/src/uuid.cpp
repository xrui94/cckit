// uuid.cpp - UUID 后端实现
// 
// 使用 Boost.UUID 作为后端

#include "cckit/uuid/uuid.h"
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
