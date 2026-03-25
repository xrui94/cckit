// vec4.h - 4D 向量 C 接口
#pragma once

#include "types.h"
#include "../../export.h"

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// Float 版本
// ========================================

CCKIT_API void cckit_vec4_set(cckit_vec4_t* v, float x, float y, float z, float w);
CCKIT_API void cckit_vec4_add(cckit_vec4_t* out, const cckit_vec4_t* a, const cckit_vec4_t* b);
CCKIT_API float cckit_vec4_dot(const cckit_vec4_t* a, const cckit_vec4_t* b);
CCKIT_API float cckit_vec4_length(const cckit_vec4_t* v);
CCKIT_API void cckit_vec4_normalize(cckit_vec4_t* v);

// ========================================
// Double 版本
// ========================================

CCKIT_API void cckit_vec4_set_d(cckit_vec4d_t* v, double x, double y, double z, double w);
CCKIT_API void cckit_vec4_add_d(cckit_vec4d_t* out, const cckit_vec4d_t* a, const cckit_vec4d_t* b);
CCKIT_API double cckit_vec4_dot_d(const cckit_vec4d_t* a, const cckit_vec4d_t* b);
CCKIT_API double cckit_vec4_length_d(const cckit_vec4d_t* v);
CCKIT_API void cckit_vec4_normalize_d(cckit_vec4d_t* v);

#ifdef __cplusplus
}
#endif
