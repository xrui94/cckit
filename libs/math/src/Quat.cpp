// quat.cpp
#include "cckit/math/capi/quat.h"
#include "cckit/export.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstring>

namespace {
    inline glm::quat to_glm(const cckit_quat_t* q) { return glm::quat(q->w, q->x, q->y, q->z); }
    inline void from_glm(cckit_quat_t* out, const glm::quat& q) { out->x = q.x; out->y = q.y; out->z = q.z; out->w = q.w; }
}

extern "C" {
void cckit_quat_identity(cckit_quat_t* q) { q->x = q->y = q->z = 0.0f; q->w = 1.0f; }
void cckit_quat_from_axis_angle(cckit_quat_t* q, const cckit_vec3_t* axis, float angle) {
    from_glm(q, glm::angleAxis(angle, glm::vec3(axis->x, axis->y, axis->z)));
}
void cckit_quat_mul(cckit_quat_t* out, const cckit_quat_t* a, const cckit_quat_t* b) { from_glm(out, to_glm(a) * to_glm(b)); }
void cckit_quat_normalize(cckit_quat_t* q) { from_glm(q, glm::normalize(to_glm(q))); }
void cckit_quat_to_mat4(cckit_mat4_t* out, const cckit_quat_t* q) {
    glm::mat4 m = glm::mat4_cast(to_glm(q));
    memcpy(out->m, glm::value_ptr(m), sizeof(float) * 16);
}
void cckit_quat_slerp(cckit_quat_t* out, const cckit_quat_t* a, const cckit_quat_t* b, float t) {
    from_glm(out, glm::slerp(to_glm(a), to_glm(b), t));
}
}
