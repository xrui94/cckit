// vec4.cpp
#include "cckit/math/capi/vec4.h"
#include "cckit/export.h"

#include <glm/glm.hpp>

namespace {
    inline glm::vec4 to_glm(const cckit_vec4_t* v) { return glm::vec4(v->x, v->y, v->z, v->w); }
    inline void from_glm(cckit_vec4_t* out, const glm::vec4& v) { out->x = v.x; out->y = v.y; out->z = v.z; out->w = v.w; }
}

extern "C" {
void cckit_vec4_set(cckit_vec4_t* v, float x, float y, float z, float w) { v->x = x; v->y = y; v->z = z; v->w = w; }
void cckit_vec4_add(cckit_vec4_t* out, const cckit_vec4_t* a, const cckit_vec4_t* b) { from_glm(out, to_glm(a) + to_glm(b)); }
float cckit_vec4_dot(const cckit_vec4_t* a, const cckit_vec4_t* b) { return glm::dot(to_glm(a), to_glm(b)); }
float cckit_vec4_length(const cckit_vec4_t* v) { return glm::length(to_glm(v)); }
void cckit_vec4_normalize(cckit_vec4_t* v) { from_glm(v, glm::normalize(to_glm(v))); }
}
