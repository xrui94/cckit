// mat3.h - 3x3 矩阵 C 接口
#pragma once

#include "types.h"
#include "../../export.h"

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// Float 版本
// ========================================

CCKIT_API void cckit_mat3_identity(cckit_mat3_t* m);
CCKIT_API void cckit_mat3_mul(cckit_mat3_t* out, const cckit_mat3_t* a, const cckit_mat3_t* b);
CCKIT_API void cckit_mat3_transpose(cckit_mat3_t* m);
CCKIT_API void cckit_mat3_inverse(cckit_mat3_t* out, const cckit_mat3_t* m);
CCKIT_API void cckit_mat3_transform_vec3(cckit_vec3_t* out, const cckit_mat3_t* m, const cckit_vec3_t* v);

// ========================================
// Double 版本
// ========================================

CCKIT_API void cckit_mat3_identity_d(cckit_mat3d_t* m);
CCKIT_API void cckit_mat3_mul_d(cckit_mat3d_t* out, const cckit_mat3d_t* a, const cckit_mat3d_t* b);
CCKIT_API void cckit_mat3_transpose_d(cckit_mat3d_t* m);
CCKIT_API void cckit_mat3_inverse_d(cckit_mat3d_t* out, const cckit_mat3d_t* m);
CCKIT_API void cckit_mat3_transform_vec3_d(cckit_vec3d_t* out, const cckit_mat3d_t* m, const cckit_vec3d_t* v);

#ifdef __cplusplus
}
#endif
