// Vec2.hpp - 2D 向量的模板化实现
//
// 这是 CCKIT 数学库的 2D 向量 C++ 接口层，提供完整的模板化支持
// 可以对标 CDTMath，支持 float 和 double 精度
//
// 设计原则：
// 1. 提供完整的模板化类型（Vec2Template<T>）
// 2. 支持数组类型（Vec2Array<T>）
// 3. 提供常用的向量运算
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
    // Vec2 - 2D 向量
    // ========================================

    template<typename T>
    class Vec2Template
    {
        static_assert(std::is_floating_point_v<T>, "Vec2 only supports floating-point types");

    public:
        T x, y;

        // 构造函数
        constexpr Vec2Template() : x(T(0)), y(T(0)) {}
        explicit constexpr Vec2Template(T v) : x(v), y(v) {}
        constexpr Vec2Template(T _x, T _y) : x(_x), y(_y) {}

        // 静态工厂
        static constexpr Vec2Template zero() { return Vec2Template(T(0), T(0)); }
        static constexpr Vec2Template one() { return Vec2Template(T(1), T(1)); }
        static constexpr Vec2Template unitX() { return Vec2Template(T(1), T(0)); }
        static constexpr Vec2Template unitY() { return Vec2Template(T(0), T(1)); }

        // 运算符
        constexpr Vec2Template operator+(const Vec2Template& other) const {
            return Vec2Template(x + other.x, y + other.y);
        }

        constexpr Vec2Template operator-(const Vec2Template& other) const {
            return Vec2Template(x - other.x, y - other.y);
        }

        constexpr T operator*(const Vec2Template& other) const { // dot product
            return x * other.x + y * other.y;
        }

        constexpr Vec2Template operator*(T s) const {
            return Vec2Template(x * s, y * s);
        }

        constexpr Vec2Template operator/(T s) const {
            const T eps = getTolerance<T>();
            if (std::abs(s) <= eps) {
                return Vec2Template(
                    std::numeric_limits<T>::quiet_NaN(),
                    std::numeric_limits<T>::quiet_NaN()
                );
            }
            return Vec2Template(x / s, y / s);
        }

        constexpr Vec2Template operator-() const {
            return Vec2Template(-x, -y);
        }

        constexpr Vec2Template& operator+=(const Vec2Template& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        constexpr Vec2Template& operator-=(const Vec2Template& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        constexpr Vec2Template& operator*=(T s) {
            x *= s;
            y *= s;
            return *this;
        }

        constexpr Vec2Template& operator/=(T s) {
            x /= s;
            y /= s;
            return *this;
        }

        // 点积
        constexpr T dot(const Vec2Template& other) const {
            return x * other.x + y * other.y;
        }

        // 长度
        T length() const {
            return std::sqrt(lengthSquared());
        }

        constexpr T lengthSquared() const {
            return x * x + y * y;
        }

        // 归一化
        Vec2Template normalized() const {
            T len = length();
            if (len > getTolerance<T>()) {
                return Vec2Template(x / len, y / len);
            }
            return zero();
        }

        void normalize() {
            T len = length();
            if (len > getTolerance<T>()) {
                x /= len;
                y /= len;
            }
        }

        // 距离
        T distance(const Vec2Template& other) const {
            return (*this - other).length();
        }

        // 线性插值
        static Vec2Template lerp(const Vec2Template& a, const Vec2Template& b, T t) {
            return a + (b - a) * t;
        }

        // 比较
        constexpr bool operator==(const Vec2Template& other) const {
            const T tol = getTolerance<T>();
            return isApproxEqual(x, other.x, tol) && isApproxEqual(y, other.y, tol);
        }

        constexpr bool operator!=(const Vec2Template& other) const {
            return !(*this == other);
        }

        // 逐分量比较（所有分量都满足条件）
        constexpr bool operator<(const Vec2Template& other) const {
            return x < other.x && y < other.y;
        }

        constexpr bool operator>(const Vec2Template& other) const {
            return x > other.x && y > other.y;
        }

        constexpr bool operator<=(const Vec2Template& other) const {
            return x <= other.x && y <= other.y;
        }

        constexpr bool operator>=(const Vec2Template& other) const {
            return x >= other.x && y >= other.y;
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
            ss << "(" << x << ", " << y << ")";
            return ss.str();
        }
    };

    template<typename T>
    inline Vec2Template<T> operator*(T s, const Vec2Template<T>& v) {
        return v * s;
    }

    // 输出流
    template<typename T>
    inline std::ostream& operator<<(std::ostream& os, const Vec2Template<T>& v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }

    // ========================================
    // 类型别名
    // ========================================

    // 无论用户怎么配置宏，Vec2 永远存在。
    // 其类型取决于 CCKIT_MATH_PRECISION_TYPE 是 float 还是 double。
    using Vec2 = Vec2Template<CCKIT_MATH_PRECISION_TYPE>;

    // 显式的浮点（ Float）/双精度（Double）版本
    using Vec2f = Vec2Template<float>;
    using Vec2d = Vec2Template<double>;

} // namespace cckit::math
