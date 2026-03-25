// mat4.h - 4x4 矩阵 C 接口
#pragma once

#include "types.h"
#include "../../export.h"

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// Float 版本
// ========================================

// 基础运算
CCKIT_API void cckit_mat4_identity(cckit_mat4_t* out);
CCKIT_API void cckit_mat4_mul(cckit_mat4_t* out, const cckit_mat4_t* a, const cckit_mat4_t* b);
CCKIT_API void cckit_mat4_transpose(cckit_mat4_t* m);
CCKIT_API bool cckit_mat4_inverse(cckit_mat4_t* out, const cckit_mat4_t* m);

// 变换
CCKIT_API void cckit_mat4_transform_vec3(cckit_vec3_t* out, const cckit_mat4_t* m, const cckit_vec3_t* v);

// 工厂函数
CCKIT_API void cckit_mat4_from_translation(cckit_mat4_t* out, const cckit_vec3_t* v);
CCKIT_API void cckit_mat4_from_rotation(cckit_mat4_t* out, float angle, const cckit_vec3_t* axis);
CCKIT_API void cckit_mat4_from_scale(cckit_mat4_t* out, const cckit_vec3_t* v);

// ========================================
// Double 版本
// ========================================

// 基础运算
CCKIT_API void cckit_mat4_identity_d(cckit_mat4d_t* out);
CCKIT_API void cckit_mat4_mul_d(cckit_mat4d_t* out, const cckit_mat4d_t* a, const cckit_mat4d_t* b);
CCKIT_API void cckit_mat4_transpose_d(cckit_mat4d_t* m);
CCKIT_API bool cckit_mat4_inverse_d(cckit_mat4d_t* out, const cckit_mat4d_t* m);

// 变换
CCKIT_API void cckit_mat4_transform_vec3_d(cckit_vec3d_t* out, const cckit_mat4d_t* m, const cckit_vec3d_t* v);

// 工厂函数
CCKIT_API void cckit_mat4_from_translation_d(cckit_mat4d_t* out, const cckit_vec3d_t* v);
CCKIT_API void cckit_mat4_from_rotation_d(cckit_mat4d_t* out, double angle, const cckit_vec3d_t* axis);
CCKIT_API void cckit_mat4_from_scale_d(cckit_mat4d_t* out, const cckit_vec3d_t* v);

#ifdef __cplusplus
}
#endif
