// Mat4.hpp - 4x4 矩阵的模板化实现
//
// 这是 CCKIT 数学库的 4x4 矩阵 C++ 接口层
// 提供完整的模板化支持，对标 CDTMath
//
// 设计原则：
// 1. 提供完整的模板化类型（Mat4Template<T>）
// 2. 保持与 CDTMath 兼容的接口设计
// 3. 支持列主序存储（OpenGL 标准）

#pragma once

#include "Vec3.hpp"
#include "Vec4.hpp"
#include "Mat3.hpp"
#include "Quat.hpp"

#include <cstring>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <array>

namespace cckit::math
{

    // ========================================
    // Mat4 - 4x4 矩阵
    // ========================================

    template<typename T>
    class Mat4Template
    {
        static_assert(std::is_floating_point_v<T>, "Mat4 only supports floating-point types");

    public:
        // 存储为列主序（column-major），与 OpenGL/GLSL 一致
        std::array<T, 16> data; // [m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32, m03, m13, m23, m33]

        // 构造函数
        constexpr Mat4Template() : Mat4Template(identity()) {}

        // 为符合数学阅读习惯，参数按"行优先"顺序传入，但内部按列主序存储
        constexpr Mat4Template(
            T m00, T m01, T m02, T m03,
            T m10, T m11, T m12, T m13,
            T m20, T m21, T m22, T m23,
            T m30, T m31, T m32, T m33
        ) : data{
            m00, m10, m20, m30,
            m01, m11, m21, m31,
            m02, m12, m22, m32,
            m03, m13, m23, m33
        } {}

        // 元素访问：符合数学习惯 mat(row, col)
        constexpr T& operator()(size_t row, size_t col) {
            return data[col * 4 + row];
        }

        constexpr const T& operator()(size_t row, size_t col) const {
            return data[col * 4 + row];
        }

        // 支持 [][] 访问（返回行指针）
        T* operator[](size_t row) {
            return &data[row * 4];
        }

        const T* operator[](size_t row) const {
            return &data[row * 4];
        }

        // 静态工厂
        static constexpr Mat4Template identity() {
            return Mat4Template(
                T(1), T(0), T(0), T(0),
                T(0), T(1), T(0), T(0),
                T(0), T(0), T(1), T(0),
                T(0), T(0), T(0), T(1)
            );
        }

        static constexpr Mat4Template zero() {
            return Mat4Template(
                T(0), T(0), T(0), T(0),
                T(0), T(0), T(0), T(0),
                T(0), T(0), T(0), T(0),
                T(0), T(0), T(0), T(0)
            );
        }

        static Mat4Template fromTranslation(const Vec3Template<T>& t) {
            auto m = identity();
            m(0, 3) = t.x;
            m(1, 3) = t.y;
            m(2, 3) = t.z;
            return m;
        }

        static Mat4Template fromScale(const Vec3Template<T>& s) {
            auto m = identity();
            m(0, 0) = s.x;
            m(1, 1) = s.y;
            m(2, 2) = s.z;
            return m;
        }

        static Mat4Template fromRotation(T angle, const Vec3Template<T>& axis) {
            Vec3Template<T> normalizedAxis = axis.normalized();
            T c = std::cos(angle);
            T s = std::sin(angle);
            T t = T(1) - c;

            T x = normalizedAxis.x;
            T y = normalizedAxis.y;
            T z = normalizedAxis.z;

            return Mat4Template(
                t * x * x + c,      t * x * y - s * z,  t * x * z + s * y,  T(0),
                t * x * y + s * z,  t * y * y + c,      t * y * z - s * x,  T(0),
                t * x * z - s * y,  t * y * z + s * x,  t * z * z + c,      T(0),
                T(0),              T(0),              T(0),              T(1)
            );
        }

        static Mat4Template fromMat3(const Mat3Template<T>& m3) {
            return Mat4Template(
                m3(0, 0), m3(0, 1), m3(0, 2), T(0),
                m3(1, 0), m3(1, 1), m3(1, 2), T(0),
                m3(2, 0), m3(2, 1), m3(2, 2), T(0),
                T(0),     T(0),     T(0),     T(1)
            );
        }

        // 从四元数创建旋转矩阵
        static Mat4Template fromRotation(const QuatTemplate<T>& q) {
            // 确保四元数已归一化
            QuatTemplate<T> normalizedQ = q.normalized();

            T xx = normalizedQ.x * normalizedQ.x;
            T yy = normalizedQ.y * normalizedQ.y;
            T zz = normalizedQ.z * normalizedQ.z;
            T xy = normalizedQ.x * normalizedQ.y;
            T xz = normalizedQ.x * normalizedQ.z;
            T yz = normalizedQ.y * normalizedQ.z;
            T wx = normalizedQ.w * normalizedQ.x;
            T wy = normalizedQ.w * normalizedQ.y;
            T wz = normalizedQ.w * normalizedQ.z;

            return Mat4Template(
                T(1) - T(2) * (yy + zz), T(2) * (xy - wz), T(2) * (xz + wy), T(0),
                T(2) * (xy + wz), T(1) - T(2) * (xx + zz), T(2) * (yz - wx), T(0),
                T(2) * (xz - wy), T(2) * (yz + wx), T(1) - T(2) * (xx + yy), T(0),
                T(0), T(0), T(0), T(1)
            );
        }

        static Mat4Template lookAt(const Vec3Template<T>& eye, const Vec3Template<T>& center, const Vec3Template<T>& up) {
            Vec3Template<T> f = (center - eye).normalized();
            Vec3Template<T> s = f.cross(up).normalized();
            Vec3Template<T> u = s.cross(f);

            return Mat4Template(
                s.x, s.y, s.z, -s.dot(eye),
                u.x, u.y, u.z, -u.dot(eye),
                -f.x, -f.y, -f.z, f.dot(eye),
                T(0), T(0), T(0), T(1)
            );
        }

        static Mat4Template perspective(T fovy, T aspect, T zNear, T zFar) {
            T tanHalfFovy = std::tan(fovy / T(2));

            Mat4Template result = Mat4Template::identity();
            result(0, 0) = T(1) / (aspect * tanHalfFovy);
            result(1, 1) = T(1) / tanHalfFovy;
            result(2, 2) = -(zFar + zNear) / (zFar - zNear);
            result(2, 3) = -(T(2) * zFar * zNear) / (zFar - zNear);
            result(3, 2) = -T(1);
            result(3, 3) = T(0);

            return result;
        }

        static Mat4Template ortho(T left, T right, T bottom, T top, T zNear, T zFar) {
            Mat4Template result = Mat4Template::identity();
            result(0, 0) = T(2) / (right - left);
            result(1, 1) = T(2) / (top - bottom);
            result(2, 2) = -T(2) / (zFar - zNear);
            result(0, 3) = -(right + left) / (right - left);
            result(1, 3) = -(top + bottom) / (top - bottom);
            result(2, 3) = -(zFar + zNear) / (zFar - zNear);

            return result;
        }

        // 矩阵运算
        constexpr Mat4Template operator+(const Mat4Template& other) const {
            Mat4Template r;
            for (int i = 0; i < 16; ++i) {
                r.data[i] = data[i] + other.data[i];
            }
            return r;
        }

        constexpr Mat4Template operator-(const Mat4Template& other) const {
            Mat4Template r;
            for (int i = 0; i < 16; ++i) {
                r.data[i] = data[i] - other.data[i];
            }
            return r;
        }

        constexpr Mat4Template operator*(const Mat4Template& other) const {
            Mat4Template res;
            for (int col = 0; col < 4; ++col) {
                for (int row = 0; row < 4; ++row) {
                    T sum = T(0);
                    for (int k = 0; k < 4; ++k) {
                        sum += (*this)(row, k) * other(k, col);
                    }
                    res(row, col) = sum;
                }
            }
            return res;
        }

        constexpr Mat4Template operator*(T s) const {
            Mat4Template r;
            for (int i = 0; i < 16; ++i) {
                r.data[i] = data[i] * s;
            }
            return r;
        }

        // 矩阵乘以向量
        constexpr Vec4Template<T> operator*(const Vec4Template<T>& v) const {
            return Vec4Template<T>(
                (*this)(0, 0) * v.x + (*this)(0, 1) * v.y + (*this)(0, 2) * v.z + (*this)(0, 3) * v.w,
                (*this)(1, 0) * v.x + (*this)(1, 1) * v.y + (*this)(1, 2) * v.z + (*this)(1, 3) * v.w,
                (*this)(2, 0) * v.x + (*this)(2, 1) * v.y + (*this)(2, 2) * v.z + (*this)(2, 3) * v.w,
                (*this)(3, 0) * v.x + (*this)(3, 1) * v.y + (*this)(3, 2) * v.z + (*this)(3, 3) * v.w
            );
        }

        constexpr Vec3Template<T> operator*(const Vec3Template<T>& v) const {
            Vec4Template<T> v4(v, T(1));
            Vec4Template<T> result = *this * v4;
            return Vec3Template<T>(result.x, result.y, result.z);
        }

        friend constexpr Mat4Template operator*(T s, const Mat4Template& m) {
            return m * s;
        }

        // 行和列访问
        constexpr std::array<T, 4> row(size_t row) const {
            return { (*this)(row, 0), (*this)(row, 1), (*this)(row, 2), (*this)(row, 3) };
        }

        constexpr std::array<T, 4> col(size_t col) const {
            return { (*this)(0, col), (*this)(1, col), (*this)(2, col), (*this)(3, col) };
        }

        // 比较
        constexpr bool operator==(const Mat4Template& other) const {
            const T tol = getTolerance<T>();
            for (size_t i = 0; i < 16; ++i) {
                if (!isApproxEqual(data[i], other.data[i], tol)) {
                    return false;
                }
            }
            return true;
        }

        constexpr bool operator!=(const Mat4Template& other) const {
            return !(*this == other);
        }

        // 点变换（齐次坐标）
        Vec3Template<T> transformPoint(const Vec3Template<T>& v) const {
            Vec4Template<T> hp = postMult(Vec4Template<T>(v.x, v.y, v.z, T(1)));
            const T eps = getTolerance<T>();
            if (isApproxZero(hp.w - T(1), eps)) {
                return Vec3Template<T>(hp.x, hp.y, hp.z);
            } else if (std::abs(hp.w) > eps) {
                return Vec3Template<T>(hp.x / hp.w, hp.y / hp.w, hp.z / hp.w);
            } else {
                return Vec3Template<T>(
                    std::numeric_limits<T>::infinity(),
                    std::numeric_limits<T>::infinity(),
                    std::numeric_limits<T>::infinity()
                );
            }
        }

        // 向量变换（忽略平移）
        Vec3Template<T> transformVector(const Vec3Template<T>& v) const {
            T x = (*this)(0, 0) * v.x + (*this)(0, 1) * v.y + (*this)(0, 2) * v.z;
            T y = (*this)(1, 0) * v.x + (*this)(1, 1) * v.y + (*this)(1, 2) * v.z;
            T z = (*this)(2, 0) * v.x + (*this)(2, 1) * v.y + (*this)(2, 2) * v.z;
            return Vec3Template<T>(x, y, z);
        }

        // 列向量左乘
        Vec4Template<T> preMult(const Vec4Template<T>& v) const {
            return Vec4Template<T>(
                v.x * (*this)(0, 0) + v.y * (*this)(1, 0) + v.z * (*this)(2, 0) + v.w * (*this)(3, 0),
                v.x * (*this)(0, 1) + v.y * (*this)(1, 1) + v.z * (*this)(2, 1) + v.w * (*this)(3, 1),
                v.x * (*this)(0, 2) + v.y * (*this)(1, 2) + v.z * (*this)(2, 2) + v.w * (*this)(3, 2),
                v.x * (*this)(0, 3) + v.y * (*this)(1, 3) + v.z * (*this)(2, 3) + v.w * (*this)(3, 3)
            );
        }

        // 行向量右乘
        Vec4Template<T> postMult(const Vec4Template<T>& v) const {
            return Vec4Template<T>(
                (*this)(0, 0) * v.x + (*this)(0, 1) * v.y + (*this)(0, 2) * v.z + (*this)(0, 3) * v.w,
                (*this)(1, 0) * v.x + (*this)(1, 1) * v.y + (*this)(1, 2) * v.z + (*this)(1, 3) * v.w,
                (*this)(2, 0) * v.x + (*this)(2, 1) * v.y + (*this)(2, 2) * v.z + (*this)(2, 3) * v.w,
                (*this)(3, 0) * v.x + (*this)(3, 1) * v.y + (*this)(3, 2) * v.z + (*this)(3, 3) * v.w
            );
        }

        Vec3Template<T> postMult(const Vec3Template<T>& v) const {
            Vec4Template<T> hp = postMult(Vec4Template<T>(v.x, v.y, v.z, T(1)));
            const T eps = getTolerance<T>();
            if (isApproxZero(hp.w, eps)) {
                return Vec3Template<T>(
                    std::numeric_limits<T>::infinity(),
                    std::numeric_limits<T>::infinity(),
                    std::numeric_limits<T>::infinity()
                );
            } else {
                T invW = T(1) / hp.w;
                return Vec3Template<T>(hp.x * invW, hp.y * invW, hp.z * invW);
            }
        }

        // 转置
        Mat4Template transpose() const {
            return Mat4Template(
                (*this)(0, 0), (*this)(1, 0), (*this)(2, 0), (*this)(3, 0),
                (*this)(0, 1), (*this)(1, 1), (*this)(2, 1), (*this)(3, 1),
                (*this)(0, 2), (*this)(1, 2), (*this)(2, 2), (*this)(3, 2),
                (*this)(0, 3), (*this)(1, 3), (*this)(2, 3), (*this)(3, 3)
            );
        }

        // 求逆（仅支持仿射矩阵）
        Mat4Template inverse() const {
            const T tol = getTolerance<T>();
            // 检查是否为仿射矩阵：最后一行必须是 [0, 0, 0, 1]
            if (!(isApproxZero((*this)(3, 0), tol) &&
                isApproxZero((*this)(3, 1), tol) &&
                isApproxZero((*this)(3, 2), tol) &&
                isApproxZero((*this)(3, 3) - T(1), tol))) {
    #ifndef NDEBUG
                assert(false && "Mat4Template::inverse() only supports affine matrices!");
    #endif
                return identity();
            }

            // 提取左上 3x3 旋转/缩放
            Mat3Template<T> rotScale(
                (*this)(0, 0), (*this)(0, 1), (*this)(0, 2),
                (*this)(1, 0), (*this)(1, 1), (*this)(1, 2),
                (*this)(2, 0), (*this)(2, 1), (*this)(2, 2)
            );

            // 平移在最后一列
            Vec3Template<T> trans((*this)(0, 3), (*this)(1, 3), (*this)(2, 3));

            Mat3Template<T> invRotScale = rotScale.inverse();
            Vec3Template<T> invTrans = invRotScale.transformVector(Vec3Template<T>(-trans.x, -trans.y, -trans.z));

            Mat4Template<T> res = fromMat3(invRotScale);
            res(0, 3) = invTrans.x;
            res(1, 3) = invTrans.y;
            res(2, 3) = invTrans.z;
            res(3, 0) = res(3, 1) = res(3, 2) = T(0);
            res(3, 3) = T(1);

            return res;
        }

        T determinant() const {
            // 使用拉普拉斯展开计算 4x4 行列式
            return
                (*this)(0, 0) * (
                    (*this)(1, 1) * ((*this)(2, 2) * (*this)(3, 3) - (*this)(3, 2) * (*this)(2, 3)) -
                    (*this)(1, 2) * ((*this)(2, 1) * (*this)(3, 3) - (*this)(3, 1) * (*this)(2, 3)) +
                    (*this)(1, 3) * ((*this)(2, 1) * (*this)(3, 2) - (*this)(3, 1) * (*this)(2, 2))
                    ) -
                (*this)(0, 1) * (
                    (*this)(1, 0) * ((*this)(2, 2) * (*this)(3, 3) - (*this)(3, 2) * (*this)(2, 3)) -
                    (*this)(1, 2) * ((*this)(2, 0) * (*this)(3, 3) - (*this)(3, 0) * (*this)(2, 3)) +
                    (*this)(1, 3) * ((*this)(2, 0) * (*this)(3, 2) - (*this)(3, 0) * (*this)(2, 2))
                    ) +
                (*this)(0, 2) * (
                    (*this)(1, 0) * ((*this)(2, 1) * (*this)(3, 3) - (*this)(3, 1) * (*this)(2, 3)) -
                    (*this)(1, 1) * ((*this)(2, 0) * (*this)(3, 3) - (*this)(3, 0) * (*this)(2, 3)) +
                    (*this)(1, 3) * ((*this)(2, 0) * (*this)(3, 1) - (*this)(3, 0) * (*this)(2, 1))
                    ) -
                (*this)(0, 3) * (
                    (*this)(1, 0) * ((*this)(2, 1) * (*this)(3, 2) - (*this)(3, 1) * (*this)(2, 2)) -
                    (*this)(1, 1) * ((*this)(2, 0) * (*this)(3, 2) - (*this)(3, 0) * (*this)(2, 2)) +
                    (*this)(1, 2) * ((*this)(2, 0) * (*this)(3, 1) - (*this)(3, 0) * (*this)(2, 1))
                    );
        }

        // 判断是否为单位矩阵
        bool isIdentity() const {
            const T tol = getTolerance<T>();
            for (size_t i = 0; i < 4; ++i) {
                for (size_t j = 0; j < 4; ++j) {
                    T expected = (i == j) ? T(1) : T(0);
                    if (!isApproxEqual((*this)(i, j), expected, tol)) {
                        return false;
                    }
                }
            }
            return true;
        }

        // 字符串表示
        std::string toString() const {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(6);
            for (int r = 0; r < 4; ++r) {
                ss << "[ ";
                for (int c = 0; c < 4; ++c) {
                    ss << (*this)(r, c);
                    if (c < 3) ss << ", ";
                }
                ss << " ]";
                if (r < 3) ss << '\n';
            }
            return ss.str();
        }
    };

    // 输出流
    template<typename T>
    inline std::ostream& operator<<(std::ostream& os, const Mat4Template<T>& m) {
        os << "[[" << m(0, 0) << ", " << m(0, 1) << ", " << m(0, 2) << ", " << m(0, 3) << "],\n"
           << " [" << m(1, 0) << ", " << m(1, 1) << ", " << m(1, 2) << ", " << m(1, 3) << "],\n"
           << " [" << m(2, 0) << ", " << m(2, 1) << ", " << m(2, 2) << ", " << m(2, 3) << "],\n"
           << " [" << m(3, 0) << ", " << m(3, 1) << ", " << m(3, 2) << ", " << m(3, 3) << "]]";
        return os;
    }

    // ========================================
    // 类型别名
    // ========================================
    // 无论用户怎么配置宏，Mat4 永远存在。
    // 其类型取决于 CCKIT_MATH_PRECISION_TYPE 是 float 还是 double。
    using Mat4 = Mat4Template<CCKIT_MATH_PRECISION_TYPE>;

    // 显式的浮点（ Float）/双精度（Double）版本
    using Mat4f = Mat4Template<float>;
    using Mat4d = Mat4Template<double>;

} // namespace cckit::math
