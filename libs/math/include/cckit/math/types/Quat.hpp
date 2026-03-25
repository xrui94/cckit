// Quat.hpp - 四元数的模板化实现
//
// 这是 CCKIT 数学库的四元数 C++ 接口层
// 提供完整的模板化支持，对标 CDTMath
//
// 设计原则：
// 1. 提供完整的模板化类型（Quat<T>）
// 2. 保持与 CDTMath 兼容的接口设计
// 3. 支持旋转变换和球面插值

#pragma once

#include "Vec.hpp"
#include "Mat.hpp"
#include "../Constants.hpp"

namespace cckit::math
{

    // ========================================
    // Quat - 四元数
    // ========================================

    template<typename T>
    class QuatTemplate
    {
        static_assert(std::is_floating_point_v<T>, "Quat only supports floating-point types");

    public:
        T x, y, z, w; // 虚部 (x, y, z) + 实部 w

        // 构造函数
        constexpr QuatTemplate() : x(T(0)), y(T(0)), z(T(0)), w(T(1)) {}

        explicit constexpr QuatTemplate(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

        // 从轴和角度构造
        constexpr QuatTemplate(const Vec3Template<T>& axis, T angle_rad) {
            T half_angle = angle_rad * T(0.5);
            T s = std::sin(half_angle);
            x = axis.x * s;
            y = axis.y * s;
            z = axis.z * s;
            w = std::cos(half_angle);
        }

        // 静态工厂
        static constexpr QuatTemplate identity() {
            return QuatTemplate(T(0), T(0), T(0), T(1));
        }

        static QuatTemplate fromAxisAngle(const Vec3Template<T>& axis, T angle) {
            return QuatTemplate(axis, angle);
        }

        static QuatTemplate fromEuler(T pitch, T yaw, T roll) {
            // 按照 ZYX 顺序（先 roll，再 yaw，最后 pitch）
            T cy = std::cos(yaw * T(0.5));
            T sy = std::sin(yaw * T(0.5));
            T cp = std::cos(pitch * T(0.5));
            T sp = std::sin(pitch * T(0.5));
            T cr = std::cos(roll * T(0.5));
            T sr = std::sin(roll * T(0.5));

            QuatTemplate q;
            q.w = cr * cp * cy + sr * sp * sy;
            q.x = sr * cp * cy - cr * sp * sy;
            q.y = cr * sp * cy + sr * cp * sy;
            q.z = cr * cp * sy - sr * sp * cy;

            return q;
        }

        static QuatTemplate fromRotationMatrix(const Mat3Template<T>& m) {
            T trace = m(0, 0) + m(1, 1) + m(2, 2);

            if (trace > T(0)) {
                T s = std::sqrt(trace + T(1)) * T(2);
                T inv_s = T(1) / s;
                return QuatTemplate(
                    (m(2, 1) - m(1, 2)) * inv_s,
                    (m(0, 2) - m(2, 0)) * inv_s,
                    (m(1, 0) - m(0, 1)) * inv_s,
                    T(0.25) * s
                );
            } else if ((m(0, 0) > m(1, 1)) && (m(0, 0) > m(2, 2))) {
                T s = std::sqrt(T(1) + m(0, 0) - m(1, 1) - m(2, 2)) * T(2);
                T inv_s = T(1) / s;
                return QuatTemplate(
                    T(0.25) * s,
                    (m(0, 1) + m(1, 0)) * inv_s,
                    (m(0, 2) + m(2, 0)) * inv_s,
                    (m(2, 1) - m(1, 2)) * inv_s
                );
            } else if (m(1, 1) > m(2, 2)) {
                T s = std::sqrt(T(1) + m(1, 1) - m(0, 0) - m(2, 2)) * T(2);
                T inv_s = T(1) / s;
                return QuatTemplate(
                    (m(0, 1) + m(1, 0)) * inv_s,
                    T(0.25) * s,
                    (m(1, 2) + m(2, 1)) * inv_s,
                    (m(0, 2) - m(2, 0)) * inv_s
                );
            } else {
                T s = std::sqrt(T(1) + m(2, 2) - m(0, 0) - m(1, 1)) * T(2);
                T inv_s = T(1) / s;
                return QuatTemplate(
                    (m(0, 2) + m(2, 0)) * inv_s,
                    (m(1, 2) + m(2, 1)) * inv_s,
                    T(0.25) * s,
                    (m(1, 0) - m(0, 1)) * inv_s
                );
            }
        }

        // 四元数乘法（Hamilton product）
        constexpr QuatTemplate operator*(const QuatTemplate& q) const {
            return QuatTemplate(
                w * q.x + x * q.w + y * q.z - z * q.y,
                w * q.y - x * q.z + y * q.w + z * q.x,
                w * q.z + x * q.y - y * q.x + z * q.w,
                w * q.w - x * q.x - y * q.y - z * q.z
            );
        }

        // 标量乘法
        constexpr QuatTemplate operator*(T s) const {
            return QuatTemplate(x * s, y * s, z * s, w * s);
        }

        friend constexpr QuatTemplate operator*(T s, const QuatTemplate& q) {
            return q * s;
        }

        // 加法
        constexpr QuatTemplate operator+(const QuatTemplate& q) const {
            return QuatTemplate(x + q.x, y + q.y, z + q.z, w + q.w);
        }

        // 减法
        constexpr QuatTemplate operator-(const QuatTemplate& q) const {
            return QuatTemplate(x - q.x, y - q.y, z - q.z, w - q.w);
        }

        // 取反
        constexpr QuatTemplate operator-() const {
            return QuatTemplate(-x, -y, -z, -w);
        }

        // 长度
        constexpr T lengthSquared() const {
            return x * x + y * y + z * z + w * w;
        }

        T length() const {
            return std::sqrt(lengthSquared());
        }

        // 归一化
        QuatTemplate normalized() const {
            T len = length();
            const T eps = getTolerance<T>();
            if (len > eps) {
                T inv = T(1) / len;
                return QuatTemplate(x * inv, y * inv, z * inv, w * inv);
            }
            return identity();
        }

        void normalize() {
            QuatTemplate q = normalized();
            x = q.x;
            y = q.y;
            z = q.z;
            w = q.w;
        }

        // 共轭
        QuatTemplate conjugate() const {
            return QuatTemplate(-x, -y, -z, w);
        }

        // 逆
        QuatTemplate inverse() const {
            T lenSq = lengthSquared();
            if (lenSq > getTolerance<T>()) {
                QuatTemplate conj = conjugate();
                T inv = T(1) / lenSq;
                return QuatTemplate(conj.x * inv, conj.y * inv, conj.z * inv, conj.w * inv);
            }
            return identity();
        }

        // 旋转向量
        Vec3Template<T> rotate(const Vec3Template<T>& v) const {
            QuatTemplate q = normalized();
            QuatTemplate vec_quat(v.x, v.y, v.z, T(0));
            QuatTemplate result = q * vec_quat * q.conjugate();
            return Vec3Template<T>(result.x, result.y, result.z);
        }

        // 转换为旋转矩阵
        Mat4Template<T> toMat4() const {
            QuatTemplate q = normalized();
            T xx = q.x * q.x, yy = q.y * q.y, zz = q.z * q.z;
            T xy = q.x * q.y, xz = q.x * q.z, yz = q.y * q.z;
            T wx = q.w * q.x, wy = q.w * q.y, wz = q.w * q.z;

            return Mat4Template<T>(
                T(1) - T(2) * (yy + zz),     T(2) * (xy - wz),           T(2) * (xz + wy),           T(0),
                T(2) * (xy + wz),            T(1) - T(2) * (xx + zz),    T(2) * (yz - wx),           T(0),
                T(2) * (xz - wy),            T(2) * (yz + wx),           T(1) - T(2) * (xx + yy),    T(0),
                T(0),                       T(0),                       T(0),                       T(1)
            );
        }

        // 球面线性插值
        static QuatTemplate slerp(const QuatTemplate& a, const QuatTemplate& b, T t) {
            T cosTheta = a.dot(b);
            QuatTemplate bAdjusted = b;

            // 选择最短路径
            if (cosTheta < T(0)) {
                cosTheta = -cosTheta;
                bAdjusted = -bAdjusted;
            }

            // 线性插值如果角度很小
            const T eps = getTolerance<T>();
            if (isApproxZero(T(1) - cosTheta, eps)) {
                return lerp(a, bAdjusted, t).normalized();
            }

            T theta = std::acos(clamp(cosTheta, T(-1), T(1)));
            T sinTheta = std::sin(theta);

            return (std::sin((T(1) - t) * theta) * a +
                    std::sin(t * theta) * bAdjusted) / sinTheta;
        }

        // 线性插值
        static QuatTemplate lerp(const QuatTemplate& a, const QuatTemplate& b, T t) {
            return a + (b - a) * t;
        }

        // 点积
        constexpr T dot(const QuatTemplate& q) const {
            return x * q.x + y * q.y + z * q.z + w * q.w;
        }

        // 比较
        constexpr bool operator==(const QuatTemplate& q) const {
            const T tol = getTolerance<T>();
            return isApproxEqual(x, q.x, tol) &&
                isApproxEqual(y, q.y, tol) &&
                isApproxEqual(z, q.z, tol) &&
                isApproxEqual(w, q.w, tol);
        }

        constexpr bool operator!=(const QuatTemplate& q) const {
            return !(*this == q);
        }

        // 转换为欧拉角
        Vec3Template<T> toEuler() const {
            QuatTemplate q = normalized();

            // Roll (x-axis rotation)
            T sinr_cosp = T(2) * (q.w * q.x + q.y * q.z);
            T cosr_cosp = T(1) - T(2) * (q.x * q.x + q.y * q.y);
            T roll = std::atan2(sinr_cosp, cosr_cosp);

            // Pitch (y-axis rotation)
            T sinp = T(2) * (q.w * q.y - q.z * q.x);
            T pitch;
            if (std::abs(sinp) >= T(1)) {
                pitch = std::copysign(PI / T(2), sinp); // use 90 degrees if out of range
            } else {
                pitch = std::asin(sinp);
            }

            // Yaw (z-axis rotation)
            T siny_cosp = T(2) * (q.w * q.z + q.x * q.y);
            T cosy_cosp = T(1) - T(2) * (q.y * q.y + q.z * q.z);
            T yaw = std::atan2(siny_cosp, cosy_cosp);

            return Vec3Template<T>(pitch, yaw, roll);
        }

        // 字符串表示
        std::string toString() const {
            std::ostringstream ss;
            ss << "(" << x << ", " << y << ", " << z << ", " << w << ")";
            return ss.str();
        }

        // 从旋转矩阵提取四元数
        static QuatTemplate fromMat4(const Mat4Template<T>& m) {
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
    };

    // ========================================
    // 类型别名
    // ========================================

    // 无论用户怎么配置宏，Quat 永远存在。
    // 其类型取决于 CCKIT_MATH_PRECISION_TYPE 是 float 还是 double。
    using Quat = QuatTemplate<CCKIT_MATH_PRECISION_TYPE>;

    // 显式的浮点（ Float）/双精度（Double）版本
    using Quatf = QuatTemplate<float>;
    using Quatd = QuatTemplate<double>;

} // namespace cckit::math
