// cckit/math/details/_Convert.hpp
#pragma once

/**
 * @brief Conversion utilities between Quat and Mat4/3.
 * @note Requires both Quat.hpp and Mat4.hpp to be included before this file.
 */

#include "types/Quat.hpp"
#include "types/Mat4.hpp"
#include "types/Mat3.hpp"

namespace cckit::math
{

    /**
     * @brief Creates a Quaternion from a 4x4 Rotation Matrix.
     * @usage: Quatf q = fromMat4(matrix);
     */
    template<typename T>
    inline QuatTemplate<T> fromMat4(const Mat4Template<T>& m) {
        // 从 4x4 矩阵提取旋转部分（3x3）
        T trace = m(0, 0) + m(1, 1) + m(2, 2);

        if (trace > T(0)) {
            T s = std::sqrt(trace + T(1)) * T(2); // s = 4 * w
            T w = T(0.25) * s;
            T x = (m(2, 1) - m(1, 2)) / s;
            T y = (m(0, 2) - m(2, 0)) / s;
            T z = (m(1, 0) - m(0, 1)) / s;
            return QuatTemplate(x, y, z, w);
        } else if ((m(0, 0) > m(1, 1)) && (m(0, 0) > m(2, 2))) {
            T s = std::sqrt(T(1) + m(0, 0) - m(1, 1) - m(2, 2)) * T(2); // s = 4 * x
            T w = (m(2, 1) - m(1, 2)) / s;
            T x = T(0.25) * s;
            T y = (m(0, 1) + m(1, 0)) / s;
            T z = (m(0, 2) + m(2, 0)) / s;
            return QuatTemplate(x, y, z, w);
        } else if (m(1, 1) > m(2, 2)) {
            T s = std::sqrt(T(1) + m(1, 1) - m(0, 0) - m(2, 2)) * T(2); // s = 4 * y
            T w = (m(0, 2) - m(2, 0)) / s;
            T x = (m(0, 1) + m(1, 0)) / s;
            T y = T(0.25) * s;
            T z = (m(1, 2) + m(2, 1)) / s;
            return QuatTemplate(x, y, z, w);
        } else {
            T s = std::sqrt(T(1) + m(2, 2) - m(0, 0) - m(1, 1)) * T(2); // s = 4 * z
            T w = (m(1, 0) - m(0, 1)) / s;
            T x = (m(0, 2) + m(2, 0)) / s;
            T y = (m(1, 2) + m(2, 1)) / s;
            T z = T(0.25) * s;
            return QuatTemplate(x, y, z, w);
        }
    }

    template<typename T>
    inline QuatTemplate<T> fromMat3(const Mat3Template<T>& m) {
        // 复用 Mat4 的逻辑，先转成 Mat4 再转 Quat，或者直接实现算法
        // 这里演示复用 Mat4 逻辑
        Mat4Template<T> m4 = Mat4Template<T>::fromMat3(m); // 假设 Mat4 有这个静态方法
        return fromMat4(m4);
    }

} // namespace cckit::math