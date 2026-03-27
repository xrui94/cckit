// uuid.h - UUID C 接口
// 
// 纯 C 接口，保证 ABI 稳定
// 后端使用 Boost.UUID 实现
#pragma once

#include "../../export.h"

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    // 【原则一】纯粹的 POD 结构体
    // UUID 是 128 位（16 字节）
    typedef struct {
        uint8_t data[16];
    } cckit_uuid_t;

    // 【原则一】纯 C 接口

    // 生成随机 UUID (v4)
    CCKIT_API void cckit_uuid_generate(cckit_uuid_t* out);

    // 检查是否为 nil UUID
    CCKIT_API bool cckit_uuid_is_nil(const cckit_uuid_t* uuid);

    // 比较
    CCKIT_API bool cckit_uuid_equal(const cckit_uuid_t* a, const cckit_uuid_t* b);

    // 转换为字符串 (格式: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx)
    CCKIT_API void cckit_uuid_to_string(const cckit_uuid_t* uuid, char* out, size_t out_size);

    // 从字符串解析
    CCKIT_API bool cckit_uuid_from_string(cckit_uuid_t* out, const char* str);

    // 哈希值（用于容器）
    CCKIT_API size_t cckit_uuid_hash(const cckit_uuid_t* uuid);

#ifdef __cplusplus
}
#endif

// 编译期检查：确保它是 POD
#ifdef __cplusplus

    #include <type_traits>

    static_assert(std::is_trivial<cckit_uuid_t>::value, "cckit_uuid_t must be trivial");
    static_assert(std::is_standard_layout<cckit_uuid_t>::value, "cckit_uuid_t must be standard layout");
    
#endif
