// vec2.cpp - 2D 向量后端实现
#include "cckit/math/capi/vec2.h"
#include "cckit/export.h"

#include <glm/glm.hpp>

namespace {
    inline glm::vec2 to_glm(const cckit_vec2_t* v) { return glm::vec2(v->x, v->y); }
    inline void from_glm(cckit_vec2_t* out, const glm::vec2& v) { out->x = v.x; out->y = v.y; }
}

extern "C" {
void cckit_vec2_set(cckit_vec2_t* v, float x, float y) { v->x = x; v->y = y; }
void cckit_vec2_add(cckit_vec2_t* out, const cckit_vec2_t* a, const cckit_vec2_t* b) { from_glm(out, to_glm(a) + to_glm(b)); }
void cckit_vec2_sub(cckit_vec2_t* out, const cckit_vec2_t* a, const cckit_vec2_t* b) { from_glm(out, to_glm(a) - to_glm(b)); }
void cckit_vec2_mul(cckit_vec2_t* out, const cckit_vec2_t* v, float s) { from_glm(out, to_glm(v) * s); }
float cckit_vec2_dot(const cckit_vec2_t* a, const cckit_vec2_t* b) { return glm::dot(to_glm(a), to_glm(b)); }
float cckit_vec2_length(const cckit_vec2_t* v) { return glm::length(to_glm(v)); }
void cckit_vec2_normalize(cckit_vec2_t* v) { from_glm(v, glm::normalize(to_glm(v))); }
}
