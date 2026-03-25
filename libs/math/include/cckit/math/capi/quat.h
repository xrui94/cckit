// quat.h - 四元数 C 接口
#pragma once

#include "types.h"
#include "../../export.h"

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// Float 版本
// ========================================

CCKIT_API void cckit_quat_identity(cckit_quat_t* q);
CCKIT_API void cckit_quat_from_axis_angle(cckit_quat_t* q, const cckit_vec3_t* axis, float angle);
CCKIT_API void cckit_quat_mul(cckit_quat_t* out, const cckit_quat_t* a, const cckit_quat_t* b);
CCKIT_API void cckit_quat_normalize(cckit_quat_t* q);
CCKIT_API void cckit_quat_to_mat4(cckit_mat4_t* out, const cckit_quat_t* q);
CCKIT_API void cckit_quat_slerp(cckit_quat_t* out, const cckit_quat_t* a, const cckit_quat_t* b, float t);

// ========================================
// Double 版本
// ========================================

CCKIT_API void cckit_quat_identity_d(cckit_quatd_t* q);
CCKIT_API void cckit_quat_from_axis_angle_d(cckit_quatd_t* q, const cckit_vec3d_t* axis, double angle);
CCKIT_API void cckit_quat_mul_d(cckit_quatd_t* out, const cckit_quatd_t* a, const cckit_quatd_t* b);
CCKIT_API void cckit_quat_normalize_d(cckit_quatd_t* q);
CCKIT_API void cckit_quat_to_mat4_d(cckit_mat4d_t* out, const cckit_quatd_t* q);
CCKIT_API void cckit_quat_slerp_d(cckit_quatd_t* out, const cckit_quatd_t* a, const cckit_quatd_t* b, double t);

#ifdef __cplusplus
}
#endif
