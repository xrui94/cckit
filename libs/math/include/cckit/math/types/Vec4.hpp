// Vec4.hpp - 4D 向量（齐次坐标）的模板化实现
//
// 这是 CCKIT 数学库的 4D 向量 C++ 接口层，提供完整的模板化支持
// 可以对标 CDTMath，支持 float 和 double 精度
//
// 设计原则：
// 1. 提供完整的模板化类型（Vec4Template<T>）
// 2. 支持数组类型（Vec4Array<T>）
// 3. 提供常用的向量运算（包括透视除法）
// 4. 保持与 CDTMath 兼容的接口设计

#pragma once

#include "Vec3.hpp"

#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <limits>
#include <type_traits>


namespace cckit::math
{

    // ========================================
    // Vec4 - 4D 向量（齐次坐标）
    // ========================================

    template<typename T>
    class Vec4Template
    {
        static_assert(std::is_floating_point_v<T>, "Vec4 only supports floating-point types");

    public:
        T x, y, z, w;

        // 构造函数
        constexpr Vec4Template() : x(T(0)), y(T(0)), z(T(0)), w(T(1)) {}
        explicit constexpr Vec4Template(T v) : x(v), y(v), z(v), w(v) {}
        constexpr Vec4Template(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

        // 从 Vec3 构造
        constexpr Vec4Template(const Vec3Template<T>& v, T _w = T(1)) : x(v.x), y(v.y), z(v.z), w(_w) {}

        // 静态工厂
        static constexpr Vec4Template zero() { return Vec4Template(T(0), T(0), T(0), T(0)); }
        static constexpr Vec4Template one() { return Vec4Template(T(1), T(1), T(1), T(1)); }
        static constexpr Vec4Template unitX() { return Vec4Template(T(1), T(0), T(0), T(0)); }
        static constexpr Vec4Template unitY() { return Vec4Template(T(0), T(1), T(0), T(0)); }
        static constexpr Vec4Template unitZ() { return Vec4Template(T(0), T(0), T(1), T(0)); }
        static constexpr Vec4Template unitW() { return Vec4Template(T(0), T(0), T(0), T(1)); }

        // 运算符
        constexpr Vec4Template operator+(const Vec4Template& other) const {
            return Vec4Template(x + other.x, y + other.y, z + other.z, w + other.w);
        }

        constexpr Vec4Template operator-(const Vec4Template& other) const {
            return Vec4Template(x - other.x, y - other.y, z - other.z, w - other.w);
        }

        constexpr T operator*(const Vec4Template& other) const { // dot product
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        constexpr Vec4Template operator*(T s) const {
            return Vec4Template(x * s, y * s, z * s, w * s);
        }

        constexpr Vec4Template operator/(T s) const {
            const T eps = getTolerance<T>();
            if (std::abs(s) <= eps) {
                return Vec4Template(
                    std::numeric_limits<T>::quiet_NaN(),
                    std::numeric_limits<T>::quiet_NaN(),
                    std::numeric_limits<T>::quiet_NaN(),
                    std::numeric_limits<T>::quiet_NaN()
                );
            }
            return Vec4Template(x / s, y / s, z / s, w / s);
        }

        constexpr Vec4Template operator-() const {
            return Vec4Template(-x, -y, -z, -w);
        }

        constexpr Vec4Template& operator+=(const Vec4Template& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        constexpr Vec4Template& operator-=(const Vec4Template& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return *this;
        }

        constexpr Vec4Template& operator*=(T s) {
            x *= s;
            y *= s;
            z *= s;
            w *= s;
            return *this;
        }

        constexpr Vec4Template& operator/=(T s) {
            x /= s;
            y /= s;
            z /= s;
            w /= s;
            return *this;
        }

        // 点积
        constexpr T dot(const Vec4Template& other) const {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        // 长度
        T length() const {
            return std::sqrt(lengthSquared());
        }

        constexpr T lengthSquared() const {
            return x * x + y * y + z * z + w * w;
        }

        // 归一化
        Vec4Template normalized() const {
            T len = length();
            if (len > getTolerance<T>()) {
                return Vec4Template(x / len, y / len, z / len, w / len);
            }
            return zero();
        }

        void normalize() {
            T len = length();
            if (len > getTolerance<T>()) {
                x /= len;
                y /= len;
                z /= len;
                w /= len;
            }
        }

        // 距离
        T distance(const Vec4Template& other) const {
            return (*this - other).length();
        }

        // 线性插值
        static Vec4Template lerp(const Vec4Template& a, const Vec4Template& b, T t) {
            return a + (b - a) * t;
        }

        // 比较
        constexpr bool operator==(const Vec4Template& other) const {
            const T tol = getTolerance<T>();
            return isApproxEqual(x, other.x, tol) &&
                   isApproxEqual(y, other.y, tol) &&
                   isApproxEqual(z, other.z, tol) &&
                   isApproxEqual(w, other.w, tol);
        }

        constexpr bool operator!=(const Vec4Template& other) const {
            return !(*this == other);
        }

        // 逐分量比较（所有分量都满足条件）
        constexpr bool operator<(const Vec4Template& other) const {
            return x < other.x && y < other.y && z < other.z && w < other.w;
        }

        constexpr bool operator>(const Vec4Template& other) const {
            return x > other.x && y > other.y && z > other.z && w > other.w;
        }

        constexpr bool operator<=(const Vec4Template& other) const {
            return x <= other.x && y <= other.y && z <= other.z && w <= other.w;
        }

        constexpr bool operator>=(const Vec4Template& other) const {
            return x >= other.x && y >= other.y && z >= other.z && w >= other.w;
        }

        // 下标访问
        T& operator[](size_t i) {
            return (&x)[i];
        }

        const T& operator[](size_t i) const {
            return (&x)[i];
        }

        // 转换为 Vec3（透视除法）
        Vec3Template<T> toVec3() const {
            if (std::abs(w) > getTolerance<T>()) {
                return Vec3Template<T>(x / w, y / w, z / w);
            }
            return Vec3Template<T>(x, y, z);
        }
    };

    template<typename T>
    inline Vec4Template<T> operator*(T s, const Vec4Template<T>& v) {
        return v * s;
    }

    // 输出流
    template<typename T>
    inline std::ostream& operator<<(std::ostream& os, const Vec4Template<T>& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
        return os;
    }

    // ========================================
    // 类型别名
    // ========================================

    // 无论用户怎么配置宏，Vec4 永远存在。
    // 其类型取决于 CCKIT_MATH_PRECISION_TYPE 是 float 还是 double。
    using Vec4 = Vec4Template<CCKIT_MATH_PRECISION_TYPE>;

    // 显式的浮点（ Float）/双精度（Double）版本
    using Vec4f = Vec4Template<float>;
    using Vec4d = Vec4Template<double>;

} // namespace cckit::math
