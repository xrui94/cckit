// Mat3.hpp - 3x3 矩阵的模板化实现
//
// 这是 CCKIT 数学库的 3x3 矩阵 C++ 接口层
// 提供完整的模板化支持，对标 CDTMath
//
// 设计原则：
// 1. 提供完整的模板化类型（Mat3Template<T>）
// 2. 保持与 CDTMath 兼容的接口设计
// 3. 支持列主序存储（OpenGL 标准）

#pragma once

#include "Vec2.hpp"

#include <cstring>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <array>

namespace cckit::math
{

    // ========================================
    // Mat3 - 3x3 矩阵
    // ========================================

    template<typename T>
    class Mat3Template
    {
        static_assert(std::is_floating_point_v<T>, "Mat3 only supports floating-point types");

    public:
        // 存储为列主序（column-major），与 OpenGL/GLSL 一致
        std::array<T, 9> data; // [m00, m10, m20, m01, m11, m21, m02, m12, m22]

        // 构造函数
        constexpr Mat3Template() : Mat3Template(identity()) {}

        // 为符合数学阅读习惯，参数按"行优先"顺序传入，但内部按列主序存储
        constexpr Mat3Template(
            T m00, T m01, T m02,
            T m10, T m11, T m12,
            T m20, T m21, T m22
        ) : data{
            m00, m10, m20,  // col0
            m01, m11, m21,  // col1
            m02, m12, m22   // col2
        } {}

        // 元素访问：符合数学习惯 mat(row, col)
        constexpr T& operator()(size_t row, size_t col) {
            return data[col * 3 + row];
        }

        constexpr const T& operator()(size_t row, size_t col) const {
            return data[col * 3 + row];
        }

        // 支持 [][] 访问（返回行指针）
        T* operator[](size_t row) {
            return &data[row * 3];
        }

        const T* operator[](size_t row) const {
            return &data[row * 3];
        }

        // 静态工厂
        static constexpr Mat3Template identity() {
            return Mat3Template(
                T(1), T(0), T(0),
                T(0), T(1), T(0),
                T(0), T(0), T(1)
            );
        }

        static constexpr Mat3Template zero() {
            return Mat3Template(
                T(0), T(0), T(0),
                T(0), T(0), T(0),
                T(0), T(0), T(0)
            );
        }

        static Mat3Template fromRotation(T angleInRad) {
            T c = std::cos(angleInRad);
            T s = std::sin(angleInRad);
            return Mat3Template(
                c, -s, T(0),
                s,  c, T(0),
                T(0), T(0), T(1)
            );
        }

        static Mat3Template fromScale(const Vec2Template<T>& scale) {
            return Mat3Template(
                scale.x, T(0), T(0),
                T(0), scale.y, T(0),
                T(0), T(0), T(1)
            );
        }

        static Mat3Template fromTranslation(const Vec2Template<T>& t) {
            return Mat3Template(
                T(1), T(0), t.x,
                T(0), T(1), t.y,
                T(0), T(0), T(1)
            );
        }

        // 矩阵运算
        constexpr Mat3Template operator+(const Mat3Template& other) const {
            Mat3Template res;
            for (size_t i = 0; i < 9; ++i) {
                res.data[i] = data[i] + other.data[i];
            }
            return res;
        }

        constexpr Mat3Template operator-(const Mat3Template& other) const {
            Mat3Template res;
            for (size_t i = 0; i < 9; ++i) {
                res.data[i] = data[i] - other.data[i];
            }
            return res;
        }

        constexpr Mat3Template operator*(const Mat3Template& other) const {
            Mat3Template res;
            for (int col = 0; col < 3; ++col) {
                for (int row = 0; row < 3; ++row) {
                    T sum = T(0);
                    for (int k = 0; k < 3; ++k) {
                        sum += (*this)(row, k) * other(k, col);
                    }
                    res(row, col) = sum;
                }
            }
            return res;
        }

        // 矩阵与向量乘法运算符
        Vec3Template<T> operator*(const Vec3Template<T>& v) const {
            return transformVector(v);
        }

        constexpr Mat3Template operator*(T scalar) const {
            Mat3Template res;
            for (size_t i = 0; i < 9; ++i) {
                res.data[i] = data[i] * scalar;
            }
            return res;
        }

        friend constexpr Mat3Template operator*(T scalar, const Mat3Template& m) {
            return m * scalar;
        }

        // 行和列访问
        constexpr std::array<T, 3> row(size_t row) const {
            return { (*this)(row, 0), (*this)(row, 1), (*this)(row, 2) };
        }

        constexpr std::array<T, 3> col(size_t col) const {
            return { (*this)(0, col), (*this)(1, col), (*this)(2, col) };
        }

        // 比较
        constexpr bool operator==(const Mat3Template& other) const {
            const T tol = getTolerance<T>();
            for (size_t i = 0; i < 9; ++i) {
                if (!isApproxEqual(data[i], other.data[i], tol)) {
                    return false;
                }
            }
            return true;
        }

        constexpr bool operator!=(const Mat3Template& other) const {
            return !(*this == other);
        }

        // 向量变换
        Vec3Template<T> transformVector(const Vec3Template<T>& v) const {
            return Vec3Template<T>(
                (*this)(0, 0) * v.x + (*this)(0, 1) * v.y + (*this)(0, 2) * v.z,
                (*this)(1, 0) * v.x + (*this)(1, 1) * v.y + (*this)(1, 2) * v.z,
                (*this)(2, 0) * v.x + (*this)(2, 1) * v.y + (*this)(2, 2) * v.z
            );
        }

        Vec3Template<T> postMult(const Vec3Template<T>& v) const {
            return transformVector(v);
        }

        // 转置
        Mat3Template transpose() const {
            return Mat3Template(
                (*this)(0, 0), (*this)(1, 0), (*this)(2, 0),
                (*this)(0, 1), (*this)(1, 1), (*this)(2, 1),
                (*this)(0, 2), (*this)(1, 2), (*this)(2, 2)
            );
        }

        // 行列式
        T determinant() const {
            return (*this)(0, 0) * ((*this)(1, 1) * (*this)(2, 2) - (*this)(2, 1) * (*this)(1, 2))
                - (*this)(1, 0) * ((*this)(0, 1) * (*this)(2, 2) - (*this)(2, 1) * (*this)(0, 2))
                + (*this)(2, 0) * ((*this)(0, 1) * (*this)(1, 2) - (*this)(1, 1) * (*this)(0, 2));
        }

        // 求逆
        Mat3Template inverse() const {
            T det = determinant();
            if (std::abs(det) <= getTolerance<T>()) {
                return identity();
            }
            T invDet = T(1) / det;

            T a00 = (*this)(0, 0), a01 = (*this)(0, 1), a02 = (*this)(0, 2);
            T a10 = (*this)(1, 0), a11 = (*this)(1, 1), a12 = (*this)(1, 2);
            T a20 = (*this)(2, 0), a21 = (*this)(2, 1), a22 = (*this)(2, 2);

            return Mat3Template(
                (a11 * a22 - a12 * a21) * invDet,
                -(a01 * a22 - a02 * a21) * invDet,
                (a01 * a12 - a02 * a11) * invDet,
                -(a10 * a22 - a12 * a20) * invDet,
                (a00 * a22 - a02 * a20) * invDet,
                -(a00 * a12 - a02 * a10) * invDet,
                (a10 * a21 - a11 * a20) * invDet,
                -(a00 * a21 - a01 * a20) * invDet,
                (a00 * a11 - a01 * a10) * invDet
            );
        }

        // 字符串表示
        std::string toString() const {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(6);
            for (int r = 0; r < 3; ++r) {
                ss << "[ ";
                for (int c = 0; c < 3; ++c) {
                    ss << (*this)(r, c);
                    if (c < 2) ss << ", ";
                }
                ss << " ]";
                if (r < 2) ss << '\n';
            }
            return ss.str();
        }
    };

    // 输出流
    template<typename T>
    inline std::ostream& operator<<(std::ostream& os, const Mat3Template<T>& m) {
        os << "[[" << m(0, 0) << ", " << m(0, 1) << ", " << m(0, 2) << "],\n"
           << " [" << m(1, 0) << ", " << m(1, 1) << ", " << m(1, 2) << "],\n"
           << " [" << m(2, 0) << ", " << m(2, 1) << ", " << m(2, 2) << "]]";
        return os;
    }

    // ========================================
    // 类型别名
    // ========================================
    // 无论用户怎么配置宏，Mat3 永远存在。
    // 其类型取决于 CCKIT_MATH_PRECISION_TYPE 是 float 还是 double。
    using Mat3 = Mat3Template<CCKIT_MATH_PRECISION_TYPE>;

    // 显式的浮点（ Float）/双精度（Double）版本
    using Mat3f = Mat3Template<float>;
    using Mat3d = Mat3Template<double>;

} // namespace cckit::math
