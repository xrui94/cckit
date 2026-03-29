// rng.h - 随机数生成器 C 接口
#pragma once

#include "types.h"
#include "../../export.h"

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// 引擎管理
// ========================================

// 创建随机数引擎（使用随机种子）
CCKIT_API cckit_rng_t* cckit_rng_create(cckit_rng_type_t type);

// 创建随机数引擎（使用指定种子）
CCKIT_API cckit_rng_t* cckit_rng_create_with_seed(cckit_rng_type_t type, uint64_t seed);

// 销毁随机数引擎
CCKIT_API void cckit_rng_destroy(cckit_rng_t* rng);

// 重新设置种子
CCKIT_API void cckit_rng_seed(cckit_rng_t* rng, uint64_t seed);

// ========================================
// 整数随机数
// ========================================

// 生成 [min, max] 范围内的随机整数
CCKIT_API int32_t cckit_rng_int(cckit_rng_t* rng, int32_t min, int32_t max);

// 生成 [min, max] 范围内的随机无符号整数
CCKIT_API uint32_t cckit_rng_uint(cckit_rng_t* rng, uint32_t min, uint32_t max);

// 生成 [min, max] 范围内的随机 64 位整数
CCKIT_API int64_t cckit_rng_int64(cckit_rng_t* rng, int64_t min, int64_t max);

// 生成 [min, max] 范围内的随机 64 位无符号整数
CCKIT_API uint64_t cckit_rng_uint64(cckit_rng_t* rng, uint64_t min, uint64_t max);

// ========================================
// 浮点随机数
// ========================================

// 生成 [min, max] 范围内的随机浮点数
CCKIT_API float cckit_rng_float(cckit_rng_t* rng, float min, float max);

// 生成 [min, max] 范围内的随机双精度浮点数
CCKIT_API double cckit_rng_double(cckit_rng_t* rng, double min, double max);

// 生成 [0, 1) 范围内的随机浮点数
CCKIT_API float cckit_rng_unit_float(cckit_rng_t* rng);

// 生成 [0, 1) 范围内的随机双精度浮点数
CCKIT_API double cckit_rng_unit_double(cckit_rng_t* rng);

// ========================================
// 正态分布
// ========================================

// 生成正态分布的随机浮点数
CCKIT_API float cckit_rng_normal_float(cckit_rng_t* rng, float mean, float stddev);

// 生成正态分布的随机双精度浮点数
CCKIT_API double cckit_rng_normal_double(cckit_rng_t* rng, double mean, double stddev);

// ========================================
// 布尔值
// ========================================

// 生成随机布尔值（50% 概率）
CCKIT_API bool cckit_rng_bool(cckit_rng_t* rng);

// 生成随机布尔值（指定概率 p 为 true）
CCKIT_API bool cckit_rng_bool_with_prob(cckit_rng_t* rng, float p);

// ========================================
// 加权随机
// ========================================

// 根据权重数组选择索引
CCKIT_API size_t cckit_rng_weighted_index(cckit_rng_t* rng, const float* weights, size_t count);

// 根据权重数组选择索引（双精度版本）
CCKIT_API size_t cckit_rng_weighted_index_d(cckit_rng_t* rng, const double* weights, size_t count);

// ========================================
// 全局引擎（便捷接口）
// ========================================

// 获取全局随机数引擎（线程安全，每个线程独立）
CCKIT_API cckit_rng_t* cckit_rng_global();

// 使用全局引擎生成 [min, max] 范围内的随机整数
CCKIT_API int32_t cckit_rand_int(int32_t min, int32_t max);

// 使用全局引擎生成 [min, max] 范围内的随机浮点数
CCKIT_API float cckit_rand_float(float min, float max);

// 使用全局引擎生成 [0, 1) 范围内的随机浮点数
CCKIT_API float cckit_rand_unit_float();

#ifdef __cplusplus
}
#endif
