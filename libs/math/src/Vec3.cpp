// vec3.cpp - 3D 向量实现 (GLM 后端)
// 
// C 接口通过 to_glm/from_glm 转换使用 GLM 功能
// C++ 用户直接使用 math.hpp 中的 GLM 类型

#include "cckit/math/capi/vec3.h"
#include "cckit/export.h"

#include <glm/glm.hpp>

namespace
{
    inline glm::vec3 to_glm(const cckit_vec3_t* v)
    {
        return glm::vec3(v->x, v->y, v->z);
    }
    
    inline void from_glm(cckit_vec3_t* out, const glm::vec3& v)
    {
        out->x = v.x;
        out->y = v.y;
        out->z = v.z;
    }
}

extern "C"
{

void cckit_vec3_add(cckit_vec3_t* out, const cckit_vec3_t* a, const cckit_vec3_t* b)
{
    from_glm(out, to_glm(a) + to_glm(b));
}

void cckit_vec3_sub(cckit_vec3_t* out, const cckit_vec3_t* a, const cckit_vec3_t* b)
{
    from_glm(out, to_glm(a) - to_glm(b));
}

void cckit_vec3_mul(cckit_vec3_t* out, const cckit_vec3_t* v, float s)
{
    from_glm(out, to_glm(v) * s);
}

void cckit_vec3_div(cckit_vec3_t* out, const cckit_vec3_t* v, float s)
{
    from_glm(out, to_glm(v) / s);
}

float cckit_vec3_dot(const cckit_vec3_t* a, const cckit_vec3_t* b)
{
    return glm::dot(to_glm(a), to_glm(b));
}

void cckit_vec3_cross(cckit_vec3_t* out, const cckit_vec3_t* a, const cckit_vec3_t* b)
{
    from_glm(out, glm::cross(to_glm(a), to_glm(b)));
}

float cckit_vec3_length(const cckit_vec3_t* v)
{
    return glm::length(to_glm(v));
}

float cckit_vec3_length_squared(const cckit_vec3_t* v)
{
    glm::vec3 gv = to_glm(v);
    return glm::dot(gv, gv);
}

void cckit_vec3_normalize(cckit_vec3_t* v)
{
    from_glm(v, glm::normalize(to_glm(v)));
}

float cckit_vec3_distance(const cckit_vec3_t* a, const cckit_vec3_t* b)
{
    return glm::distance(to_glm(a), to_glm(b));
}

void cckit_vec3_lerp(cckit_vec3_t* out, const cckit_vec3_t* a, const cckit_vec3_t* b, float t)
{
    from_glm(out, glm::mix(to_glm(a), to_glm(b), t));
}

bool cckit_vec3_equal(const cckit_vec3_t* a, const cckit_vec3_t* b)
{
    return a->x == b->x && a->y == b->y && a->z == b->z;
}

} // extern "C"
