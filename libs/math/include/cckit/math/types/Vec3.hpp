// Vec3.hpp - 3D 向量的模板化实现
//
// 这是 CCKIT 数学库的 3D 向量 C++ 接口层，提供完整的模板化支持
// 可以对标 CDTMath，支持 float 和 double 精度
//
// 设计原则：
// 1. 提供完整的模板化类型（Vec3Template<T>）
// 2. 支持数组类型（Vec3Array<T>）
// 3. 提供常用的向量运算（包括叉积）
// 4. 保持与 CDTMath 兼容的接口设计

#pragma once

#include "../MathConfig.h"
#include "../Tolerance.hpp"

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
    // Vec3 - 3D 向量
    // ========================================

    template<typename T>
    class Vec3Template
    {
        static_assert(std::is_floating_point_v<T>, "Vec3 only supports floating-point types");

    public:
        T x, y, z;

        // 构造函数
        constexpr Vec3Template() : x(T(0)), y(T(0)), z(T(0)) {}
        explicit constexpr Vec3Template(T v) : x(v), y(v), z(v) {}
        constexpr Vec3Template(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

        // 静态工厂
        static constexpr Vec3Template zero() { return Vec3Template(T(0), T(0), T(0)); }
        static constexpr Vec3Template one() { return Vec3Template(T(1), T(1), T(1)); }
        static constexpr Vec3Template unitX() { return Vec3Template(T(1), T(0), T(0)); }
        static constexpr Vec3Template unitY() { return Vec3Template(T(0), T(1), T(0)); }
        static constexpr Vec3Template unitZ() { return Vec3Template(T(0), T(0), T(1)); }

        // 运算符
        constexpr Vec3Template operator+(const Vec3Template& other) const {
            return Vec3Template(x + other.x, y + other.y, z + other.z);
        }

        constexpr Vec3Template operator-(const Vec3Template& other) const {
            return Vec3Template(x - other.x, y - other.y, z - other.z);
        }

        constexpr T operator*(const Vec3Template& other) const { // dot product
            return x * other.x + y * other.y + z * other.z;
        }

        constexpr Vec3Template operator*(T s) const {
            return Vec3Template(x * s, y * s, z * s);
        }

        constexpr Vec3Template operator/(T s) const {
            const T eps = getTolerance<T>();
            if (std::abs(s) <= eps) {
                return Vec3Template(
                    std::numeric_limits<T>::quiet_NaN(),
                    std::numeric_limits<T>::quiet_NaN(),
                    std::numeric_limits<T>::quiet_NaN()
                );
            }
            return Vec3Template(x / s, y / s, z / s);
        }

        constexpr Vec3Template operator-() const {
            return Vec3Template(-x, -y, -z);
        }

        constexpr Vec3Template& operator+=(const Vec3Template& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        constexpr Vec3Template& operator-=(const Vec3Template& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        constexpr Vec3Template& operator*=(T s) {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }

        constexpr Vec3Template& operator/=(T s) {
            x /= s;
            y /= s;
            z /= s;
            return *this;
        }

        // 点积
        constexpr T dot(const Vec3Template& other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        // 叉积
        Vec3Template cross(const Vec3Template& other) const {
            return Vec3Template(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            );
        }

        // 长度
        T length() const {
            return std::sqrt(lengthSquared());
        }

        constexpr T lengthSquared() const {
            return x * x + y * y + z * z;
        }

        // 归一化
        Vec3Template normalized() const {
            T len = length();
            if (len > getTolerance<T>()) {
                return Vec3Template(x / len, y / len, z / len);
            }
            return zero();
        }

        void normalize() {
            T len = length();
            if (len > getTolerance<T>()) {
                x /= len;
                y /= len;
                z /= len;
            }
        }

        // 距离
        T distance(const Vec3Template& other) const {
            return (*this - other).length();
        }

        // 线性插值
        static Vec3Template lerp(const Vec3Template& a, const Vec3Template& b, T t) {
            return a + (b - a) * t;
        }

        // 比较
        constexpr bool operator==(const Vec3Template& other) const {
            const T tol = getTolerance<T>();
            return isApproxEqual(x, other.x, tol) &&
                isApproxEqual(y, other.y, tol) &&
                isApproxEqual(z, other.z, tol);
        }

        constexpr bool operator!=(const Vec3Template& other) const {
            return !(*this == other);
        }

        // 下标访问
        T& operator[](size_t i) {
            return (&x)[i];
        }

        const T& operator[](size_t i) const {
            return (&x)[i];
        }

        // 转换为字符串
        std::string toString() const {
            std::ostringstream ss;
            ss << "(" << x << ", " << y << ", " << z << ")";
            return ss.str();
        }
    };

    template<typename T>
    inline Vec3Template<T> operator*(T s, const Vec3Template<T>& v) {
        return v * s;
    }

    // 输出流
    template<typename T>
    inline std::ostream& operator<<(std::ostream& os, const Vec3Template<T>& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }

    // ========================================
    // 类型别名
    // ========================================

    // 无论用户怎么配置宏，Vec3 永远存在。
    // 其类型取决于 CCKIT_MATH_PRECISION_TYPE 是 float 还是 double。
    using Vec3 = Vec3Template<CCKIT_MATH_PRECISION_TYPE>;

    // 显式的浮点（ Float）/双精度（Double）版本
    using Vec3f = Vec3Template<float>;
    using Vec3d = Vec3Template<double>;

} // namespace math::cckit
