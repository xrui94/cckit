// uuid.hpp - UUID C++ 包装层
// 
// 提供类型安全的 C++ 接口
// 内部直接持有 POD，保证 ABI 稳定
#pragma once

#include "uuid.h"
#include <string>
#include <optional>
#include <functional>

namespace cckit {

/**
 * @brief UUID 类
 * 
 * 【原则二】值语义类，内部直接持有 POD
 * 保证 ABI 稳定，可以安全地跨模块边界传递
 */
class Uuid
{
private:
    cckit_uuid_t _data;

public:
    // 默认构造（nil UUID）
    constexpr Uuid() : _data{{0}} {}
    
    // 从 POD 构造
    explicit constexpr Uuid(const cckit_uuid_t& data) : _data(data) {}
    
    // 生成随机 UUID
    static Uuid generate()
    {
        Uuid result;
        cckit_uuid_generate(&result._data);
        return result;
    }
    
    // 从字符串解析
    static std::optional<Uuid> fromString(const std::string& str)
    {
        Uuid result;
        if (cckit_uuid_from_string(&result._data, str.c_str()))
        {
            return result;
        }
        return std::nullopt;
    }
    
    // 检查是否为 nil
    bool isNil() const
    {
        return cckit_uuid_is_nil(&_data);
    }
    
    // 转换为字符串
    std::string toString() const
    {
        char buf[37]; // 36 chars + null terminator
        cckit_uuid_to_string(&_data, buf, sizeof(buf));
        return std::string(buf);
    }
    
    // 比较
    bool operator==(const Uuid& other) const
    {
        return cckit_uuid_equal(&_data, &other._data);
    }
    
    bool operator!=(const Uuid& other) const
    {
        return !(*this == other);
    }
    
    bool operator<(const Uuid& other) const
    {
        // 字典序比较
        for (int i = 0; i < 16; ++i)
        {
            if (_data.data[i] < other._data.data[i]) return true;
            if (_data.data[i] > other._data.data[i]) return false;
        }
        return false;
    }
    
    // 隐式转换：允许无缝传递给需要底层 POD 的 C API
    operator const cckit_uuid_t&() const { return _data; }
    operator cckit_uuid_t&() { return _data; }
    
    // 获取底层 POD
    const cckit_uuid_t& data() const { return _data; }
    cckit_uuid_t& data() { return _data; }
};

// 便捷函数
inline Uuid genUuid()
{
    return Uuid::generate();
}

} // namespace cckit

// 哈希支持（用于 std::unordered_map 等容器）
namespace std
{
    template<>
    struct hash<cckit::Uuid>
    {
        size_t operator()(const cckit::Uuid& uuid) const noexcept
        {
            return cckit_uuid_hash(&uuid.data());
        }
    };
}
