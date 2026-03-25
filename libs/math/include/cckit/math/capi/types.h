// types.h - 数学类型定义 (POD)
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// POD 类型定义
// ========================================

// Float 类型
typedef struct { float x, y; } cckit_vec2f_t;
typedef struct { float x, y, z; } cckit_vec3f_t;
typedef struct { float x, y, z, w; } cckit_vec4f_t;

// Double 类型
typedef struct { double x, y; } cckit_vec2d_t;
typedef struct { double x, y, z; } cckit_vec3d_t;
typedef struct { double x, y, z, w; } cckit_vec4d_t;

// 矩阵：列主序 (OpenGL 约定)
typedef struct { float m[4]; } cckit_mat2f_t;    // 2x2
typedef struct { float m[9]; } cckit_mat3f_t;    // 3x3
typedef struct { float m[16]; } cckit_mat4f_t;   // 4x4

typedef struct { double m[4]; } cckit_mat2d_t;   // 2x2
typedef struct { double m[9]; } cckit_mat3d_t;   // 3x3
typedef struct { double m[16]; } cckit_mat4d_t;  // 4x4

// 四元数： + 虚部
typedef struct { float w, x, y, z; } cckit_quatf_t;
typedef struct { double w, x, y, z; } cckit_quatd_t;

// 默认类型别名（保持向后兼容）
typedef cckit_vec2f_t cckit_vec2_t;
typedef cckit_vec3f_t cckit_vec3_t;
typedef cckit_vec4f_t cckit_vec4_t;
typedef cckit_mat2f_t cckit_mat2_t;
typedef cckit_mat3f_t cckit_mat3_t;
typedef cckit_mat4f_t cckit_mat4_t;
typedef cckit_quatf_t cckit_quat_t;

// POD 验证
#ifdef __cplusplus
static_assert(sizeof(cckit_vec2f_t) == 8, "cckit_vec2f_t must be POD");
static_assert(sizeof(cckit_vec3f_t) == 12, "cckit_vec3f_t must be POD");
static_assert(sizeof(cckit_vec4f_t) == 16, "cckit_vec4f_t must be POD");
static_assert(sizeof(cckit_vec2d_t) == 16, "cckit_vec2d_t must be POD");
static_assert(sizeof(cckit_vec3d_t) == 24, "cckit_vec3d_t must be POD");
static_assert(sizeof(cckit_vec4d_t) == 32, "cckit_vec4d_t must be POD");

static_assert(sizeof(cckit_mat4f_t) == 64, "cckit_mat4f_t must be POD");
static_assert(sizeof(cckit_mat4d_t) == 128, "cckit_mat4d_t must be POD");

static_assert(sizeof(cckit_quatf_t) == 16, "cckit_quatf_t must be POD");
static_assert(sizeof(cckit_quatd_t) == 32, "cckit_quatd_t must be POD");
#endif

#ifdef __cplusplus
}
#endif
