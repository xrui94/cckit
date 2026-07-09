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
    // 前向声明，供 transform/transformed 使用（实现在 exts/Vec3Ext.hpp）
    template<typename T> class Mat4Template;
    template<typename T> class Mat3Template;

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

        // ========================================
        // 空间变换（原地修改，返回自身引用以支持链式调用）
        // ========================================

        // 绕轴旋转（Rodrigues公式），axis 自动归一化，绕原点
        Vec3Template& rotate(T angle_rad, const Vec3Template& axis) {
            Vec3Template u = axis.normalized();
            T c = std::cos(angle_rad);
            T s = std::sin(angle_rad);
            T t = T(1) - c;
            T dotVal = x * u.x + y * u.y + z * u.z;
            Vec3Template result(
                x * c + (u.y * z - u.z * y) * s + u.x * dotVal * t,
                y * c + (u.z * x - u.x * z) * s + u.y * dotVal * t,
                z * c + (u.x * y - u.y * x) * s + u.z * dotVal * t
            );
            x = result.x; y = result.y; z = result.z;
            return *this;
        }

        // 绕轴旋转，轴过指定点 axis_origin（对应 OCCT gp_Ax1）
        Vec3Template& rotate(const Vec3Template& axis_origin, const Vec3Template& axis_dir, T angle_rad) {
            *this -= axis_origin;
            this->rotate(angle_rad, axis_dir);
            *this += axis_origin;
            return *this;
        }

        Vec3Template rotated(T angle_rad, const Vec3Template& axis) const {
            return Vec3Template(*this).rotate(angle_rad, axis);
        }

        Vec3Template rotated(const Vec3Template& axis_origin, const Vec3Template& axis_dir, T angle_rad) const {
            return Vec3Template(*this).rotate(axis_origin, axis_dir, angle_rad);
        }

        // 均匀缩放
        Vec3Template& scale(T factor) {
            x *= factor; y *= factor; z *= factor;
            return *this;
        }

        // 逐轴缩放
        Vec3Template& scale(const Vec3Template& factors) {
            x *= factors.x; y *= factors.y; z *= factors.z;
            return *this;
        }

        // 绕枢轴点缩放（对应 OCCT gp_Pnt::Scale）
        Vec3Template& scale(const Vec3Template& pivot, T s) {
            x = x * s + pivot.x * (T(1) - s);
            y = y * s + pivot.y * (T(1) - s);
            z = z * s + pivot.z * (T(1) - s);
            return *this;
        }

        Vec3Template scaled(T factor) const {
            return Vec3Template(*this).scale(factor);
        }

        Vec3Template scaled(const Vec3Template& factors) const {
            return Vec3Template(*this).scale(factors);
        }

        Vec3Template scaled(const Vec3Template& pivot, T s) const {
            return Vec3Template(*this).scale(pivot, s);
        }

        // 平移
        Vec3Template& translate(const Vec3Template& offset) {
            x += offset.x; y += offset.y; z += offset.z;
            return *this;
        }

        // 从点 p1 平移到点 p2（等效于加 (p2 - p1)），对应 OCCT gp_Pnt::Translate(p1, p2)
        Vec3Template& translate(const Vec3Template& p1, const Vec3Template& p2) {
            x += p2.x - p1.x;
            y += p2.y - p1.y;
            z += p2.z - p1.z;
            return *this;
        }

        
        Vec3Template translated(const Vec3Template& offset) const {
            return Vec3Template(*this).translate(offset);
        }

        Vec3Template translated(const Vec3Template& p1, const Vec3Template& p2) const {
            return Vec3Template(*this).translate(p1, p2);
        }

        // 矩阵变换（依赖 Mat4Template/Mat3Template，实现在 exts/Vec3Ext.hpp）
        Vec3Template& transform(const Mat4Template<T>& m);

        Vec3Template& transform(const Mat3Template<T>& m);

        Vec3Template transformed(const Mat4Template<T>& m) const;

        Vec3Template transformed(const Mat3Template<T>& m) const;

        // 归一化
        Vec3Template normalized() const {
            T len = length();
            if (len > getTolerance<T>()) {
                return Vec3Template(x / len, y / len, z / len);
            }
            return zero();
        }

        // 对自身进行归一化
        void normalize() {
            T len = length();
            if (len > getTolerance<T>()) {
                x /= len;
                y /= len;
                z /= len;
            }
        }

        // 长度
        T length() const {
            return std::sqrt(lengthSquared());
        }

        constexpr T lengthSquared() const {
            return x * x + y * y + z * z;
        }

        // 计算向量的模长（magnitude），与 length() 相同，只是方法名不同而已
        T magnitude() const {
            return length();
        }

        // 距离
        T distance(const Vec3Template& other) const {
            return (*this - other).length();
        }

        // 计算两点间距离的平方（极力推荐！在比较距离大小时，省去开方操作，性能提升显著）
        constexpr T distanceSquared(const Vec3Template& other) const {
            return (*this - other).lengthSquared();
        }

        /**
        * @brief 计算两个三维向量之间的夹角（弧度制）
        * @param other   第二个向量
        * @return 夹角弧度值 [0, PI]
        */
        T angle(const Vec3Template & other) const {
           T lenProduct = length() * other.length();
           // 1. 防零除保护
           if (lenProduct < getTolerance<T>()) {
               return T(0);
           }

           T cosTheta = dot(other) / lenProduct;
           // 2. Clamp 防止浮点误差导致 NaN
           cosTheta = std::max(T(-1), std::min(T(1), cosTheta));

           // 3. 移植 OCCT 的混合精度算法
           // 0.70710678118655 是 sqrt(2)/2，即 45 度的余弦值
           constexpr T THRESHOLD = T(0.70710678118655);

           if (cosTheta > -THRESHOLD && cosTheta < THRESHOLD) {
               // 角度在 45° ~ 135° 之间，使用 acos 精度最高
               return std::acos(cosTheta);
           }
           else {
               // 角度在 0° ~ 45° 或 135° ~ 180° 之间，使用 asin 精度最高
               // sin(theta) = |a x b| / (|a| * |b|)
               T sinTheta = cross(other).length() / lenProduct;
               sinTheta = std::min(T(1), sinTheta); // 同样做 Clamp 保护

               if (cosTheta < T(0)) {
                   // 钝角 (90° ~ 180°)
                   return T(M_PI) - std::asin(sinTheta);
               }
               else {
                   // 锐角 (0° ~ 90°)
                   return std::asin(sinTheta);
               }
           }
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

        // 逐分量比较（所有分量都满足条件）
        constexpr bool operator<(const Vec3Template& other) const {
            return x < other.x && y < other.y && z < other.z;
        }

        constexpr bool operator>(const Vec3Template& other) const {
            return x > other.x && y > other.y && z > other.z;
        }

        constexpr bool operator<=(const Vec3Template& other) const {
            return x <= other.x && y <= other.y && z <= other.z;
        }

        constexpr bool operator>=(const Vec3Template& other) const {
            return x >= other.x && y >= other.y && z >= other.z;
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
