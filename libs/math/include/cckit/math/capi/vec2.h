// vec2.h - 2D 向量 C 接口
#pragma once

#include "types.h"
#include "../../export.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// Float 版本
// ========================================

CCKIT_API void cckit_vec2_set(cckit_vec2_t* v, float x, float y);
CCKIT_API void cckit_vec2_add(cckit_vec2_t* out, const cckit_vec2_t* a, const cckit_vec2_t* b);
CCKIT_API void cckit_vec2_sub(cckit_vec2_t* out, const cckit_vec2_t* a, const cckit_vec2_t* b);
CCKIT_API void cckit_vec2_mul(cckit_vec2_t* out, const cckit_vec2_t* v, float s);
CCKIT_API float cckit_vec2_dot(const cckit_vec2_t* a, const cckit_vec2_t* b);
CCKIT_API float cckit_vec2_length(const cckit_vec2_t* v);
CCKIT_API void cckit_vec2_normalize(cckit_vec2_t* v);

// ========================================
// Double 版本
// ========================================

CCKIT_API void cckit_vec2_set_d(cckit_vec2d_t* v, double x, double y);
CCKIT_API void cckit_vec2_add_d(cckit_vec2d_t* out, const cckit_vec2d_t* a, const cckit_vec2d_t* b);
CCKIT_API void cckit_vec2_sub_d(cckit_vec2d_t* out, const cckit_vec2d_t* a, const cckit_vec2d_t* b);
CCKIT_API void cckit_vec2_mul_d(cckit_vec2d_t* out, const cckit_vec2d_t* v, double s);
CCKIT_API double cckit_vec2_dot_d(const cckit_vec2d_t* a, const cckit_vec2d_t* b);
CCKIT_API double cckit_vec2_length_d(const cckit_vec2d_t* v);
CCKIT_API void cckit_vec2_normalize_d(cckit_vec2d_t* v);

#ifdef __cplusplus
}
#endif
