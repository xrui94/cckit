// mat4.cpp - 4x4 矩阵实现 (GLM 后端)
// 
// C 接口通过 to_glm/from_glm 转换使用 GLM 功能
// C++ 用户直接使用 math.hpp 中的 GLM 类型

#include "cckit/math/capi/mat4.h"
#include "cckit/export.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{
    inline glm::mat4 to_glm(const cckit_mat4_t* m)
    {
        // GLM 矩阵是列主序，与 cckit_mat4_t 一致
        return glm::make_mat4(m->m);
    }
    
    inline void from_glm(cckit_mat4_t* out, const glm::mat4& m)
    {
        const float* ptr = glm::value_ptr(m);
        std::memcpy(out->m, ptr, sizeof(float) * 16);
    }
    
    inline glm::vec3 to_glm_vec3(const cckit_vec3_t* v)
    {
        return glm::vec3(v->x, v->y, v->z);
    }
}

extern "C"
{

void cckit_mat4_identity(cckit_mat4_t* out)
{
    from_glm(out, glm::mat4(1.0f));
}

void cckit_mat4_mul(cckit_mat4_t* out, const cckit_mat4_t* a, const cckit_mat4_t* b)
{
    from_glm(out, to_glm(a) * to_glm(b));
}

void cckit_mat4_transpose(cckit_mat4_t* m)
{
    glm::mat4 result = glm::transpose(to_glm(m));
    from_glm(m, result);
}

bool cckit_mat4_inverse(cckit_mat4_t* out, const cckit_mat4_t* m)
{
    glm::mat4 gm = to_glm(m);
    float det = glm::determinant(gm);
    if (std::abs(det) < 1e-10f)
    {
        from_glm(out, glm::mat4(1.0f));
        return false;
    }
    from_glm(out, glm::inverse(gm));
    return true;
}

void cckit_mat4_transform_vec3(cckit_vec3_t* out, const cckit_mat4_t* m, const cckit_vec3_t* v)
{
    glm::vec4 result = to_glm(m) * glm::vec4(v->x, v->y, v->z, 1.0f);
    if (result.w != 0.0f && result.w != 1.0f)
    {
        float invW = 1.0f / result.w;
        out->x = result.x * invW;
        out->y = result.y * invW;
        out->z = result.z * invW;
    }
    else
    {
        out->x = result.x;
        out->y = result.y;
        out->z = result.z;
    }
}

void cckit_mat4_from_translation(cckit_mat4_t* out, const cckit_vec3_t* v)
{
    from_glm(out, glm::translate(glm::mat4(1.0f), to_glm_vec3(v)));
}

void cckit_mat4_from_rotation(cckit_mat4_t* out, float angle, const cckit_vec3_t* axis)
{
    from_glm(out, glm::rotate(glm::mat4(1.0f), angle, to_glm_vec3(axis)));
}

void cckit_mat4_from_scale(cckit_mat4_t* out, const cckit_vec3_t* v)
{
    from_glm(out, glm::scale(glm::mat4(1.0f), to_glm_vec3(v)));
}

} // extern "C"
