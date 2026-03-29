// types.h - 随机数类型定义 (POD)
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// POD 类型定义
// ========================================

// 随机数引擎状态句柄（不透明指针）
typedef struct cckit_rng_t cckit_rng_t;

// 随机数引擎类型
typedef enum {
    CCKIT_RNG_MT19937,      // Mersenne Twister (默认)
    CCKIT_RNG_MT19937_64,   // 64-bit Mersenne Twister
    CCKIT_RNG_MINSTD_RAND,  // Linear congruential
    CCKIT_RNG_KNUTH_B       // Knuth's subtract_with_carry
} cckit_rng_type_t;

// POD 验证
#ifdef __cplusplus
static_assert(sizeof(cckit_rng_type_t) == 4, "cckit_rng_type_t must be POD");
#endif

#ifdef __cplusplus
}
#endif
