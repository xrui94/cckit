// mat3.cpp
#include "cckit/math/capi/mat3.h"
#include "cckit/export.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

namespace {
    inline glm::mat3 to_glm(const cckit_mat3_t* m) { return glm::make_mat3(m->m); }
    inline void from_glm(cckit_mat3_t* out, const glm::mat3& m) { memcpy(out->m, glm::value_ptr(m), sizeof(float) * 9); }
}

extern "C" {
    void cckit_mat3_identity(cckit_mat3_t* m) { from_glm(m, glm::mat3(1.0f)); }
    void cckit_mat3_mul(cckit_mat3_t* out, const cckit_mat3_t* a, const cckit_mat3_t* b) { from_glm(out, to_glm(a) * to_glm(b)); }
    void cckit_mat3_transpose(cckit_mat3_t* m) { from_glm(m, glm::transpose(to_glm(m))); }
    void cckit_mat3_inverse(cckit_mat3_t* out, const cckit_mat3_t* m) { from_glm(out, glm::inverse(to_glm(m))); }
    void cckit_mat3_transform_vec3(cckit_vec3_t* out, const cckit_mat3_t* m, const cckit_vec3_t* v) {
        glm::vec3 result = to_glm(m) * glm::vec3(v->x, v->y, v->z);
        out->x = result.x; out->y = result.y; out->z = result.z;
    }
}
