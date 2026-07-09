// Vec3Ext.hpp - Vec3 与 Mat4/Mat3 的扩展转换（解决循环包含）
//
// 由于 Vec3.hpp 和 Mat4.hpp 存在互相包含的依赖关系（Mat4 需要 Vec3，
// Vec3 的 transform 方法需要 Mat4），这些方法在此文件中延迟定义。
// 此文件必须在 Mat4.hpp 之后被包含。
//
// Math.hpp 的包含顺序：Vec.hpp -> Mat.hpp -> Quat.hpp -> ... -> Vec3Ext.hpp

#pragma once

#include "../types/Vec3.hpp"
#include "../types/Mat4.hpp"
#include "../types/Mat3.hpp"

namespace cckit::math
{

// ========================================
// Vec3Template::transform (原地修改)
// ========================================

template<typename T>
Vec3Template<T>& Vec3Template<T>::transform(const Mat4Template<T>& m) {
    // 将 Vec3 视为点（w=1），做齐次坐标变换，结果做透视除法
    T nx = m(0, 0) * x + m(0, 1) * y + m(0, 2) * z + m(0, 3);
    T ny = m(1, 0) * x + m(1, 1) * y + m(1, 2) * z + m(1, 3);
    T nz = m(2, 0) * x + m(2, 1) * y + m(2, 2) * z + m(2, 3);
    T nw = m(3, 0) * x + m(3, 1) * y + m(3, 2) * z + m(3, 3);
    if (std::abs(nw - T(1)) > getTolerance<T>() && std::abs(nw) > getTolerance<T>()) {
        T invW = T(1) / nw;
        x = nx * invW;
        y = ny * invW;
        z = nz * invW;
    } else {
        x = nx;
        y = ny;
        z = nz;
    }
    return *this;
}

template<typename T>
Vec3Template<T>& Vec3Template<T>::transform(const Mat3Template<T>& m) {
    T nx = m(0, 0) * x + m(0, 1) * y + m(0, 2) * z;
    T ny = m(1, 0) * x + m(1, 1) * y + m(1, 2) * z;
    T nz = m(2, 0) * x + m(2, 1) * y + m(2, 2) * z;
    x = nx;
    y = ny;
    z = nz;
    return *this;
}

// ========================================
// Vec3Template::transformed (返回新向量)
// ========================================

template<typename T>
Vec3Template<T> Vec3Template<T>::transformed(const Mat4Template<T>& m) const {
    return Vec3Template(*this).transform(m);
}

template<typename T>
Vec3Template<T> Vec3Template<T>::transformed(const Mat3Template<T>& m) const {
    return Vec3Template(*this).transform(m);
}

} // namespace cckit::math
