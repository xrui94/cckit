// vec3.h - 3D 向量 C 接口
#pragma once

#include "types.h"
#include "../../export.h"

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// Float 版本
// ========================================

CCKIT_API void cckit_vec3_add(cckit_vec3_t* out, const cckit_vec3_t* a, const cckit_vec3_t* b);
CCKIT_API void cckit_vec3_sub(cckit_vec3_t* out, const cckit_vec3_t* a, const cckit_vec3_t* b);
CCKIT_API void cckit_vec3_mul(cckit_vec3_t* out, const cckit_vec3_t* v, float s);
CCKIT_API void cckit_vec3_div(cckit_vec3_t* out, const cckit_vec3_t* v, float s);

CCKIT_API float cckit_vec3_dot(const cckit_vec3_t* a, const cckit_vec3_t* b);
CCKIT_API void cckit_vec3_cross(cckit_vec3_t* out, const cckit_vec3_t* a, const cckit_vec3_t* b);

CCKIT_API float cckit_vec3_length(const cckit_vec3_t* v);
CCKIT_API float cckit_vec3_length_squared(const cckit_vec3_t* v);
CCKIT_API void cckit_vec3_normalize(cckit_vec3_t* v);

CCKIT_API float cckit_vec3_distance(const cckit_vec3_t* a, const cckit_vec3_t* b);
CCKIT_API void cckit_vec3_lerp(cckit_vec3_t* out, const cckit_vec3_t* a, const cckit_vec3_t* b, float t);

CCKIT_API bool cckit_vec3_equal(const cckit_vec3_t* a, const cckit_vec3_t* b);

// ========================================
// Double 版本
// ========================================

CCKIT_API void cckit_vec3_add_d(cckit_vec3d_t* out, const cckit_vec3d_t* a, const cckit_vec3d_t* b);
CCKIT_API void cckit_vec3_sub_d(cckit_vec3d_t* out, const cckit_vec3d_t* a, const cckit_vec3d_t* b);
CCKIT_API void cckit_vec3_mul_d(cckit_vec3d_t* out, const cckit_vec3d_t* v, double s);
CCKIT_API void cckit_vec3_div_d(cckit_vec3d_t* out, const cckit_vec3d_t* v, double s);

CCKIT_API double cckit_vec3_dot_d(const cckit_vec3d_t* a, const cckit_vec3d_t* b);
CCKIT_API void cckit_vec3_cross_d(cckit_vec3d_t* out, const cckit_vec3d_t* a, const cckit_vec3d_t* b);

CCKIT_API double cckit_vec3_length_d(const cckit_vec3d_t* v);
CCKIT_API double cckit_vec3_length_squared_d(const cckit_vec3d_t* v);
CCKIT_API void cckit_vec3_normalize_d(cckit_vec3d_t* v);

CCKIT_API double cckit_vec3_distance_d(const cckit_vec3d_t* a, const cckit_vec3d_t* b);
CCKIT_API void cckit_vec3_lerp_d(cckit_vec3d_t* out, const cckit_vec3d_t* a, const cckit_vec3d_t* b, double t);

CCKIT_API bool cckit_vec3_equal_d(const cckit_vec3d_t* a, const cckit_vec3d_t* b);

#ifdef __cplusplus
}
#endif
