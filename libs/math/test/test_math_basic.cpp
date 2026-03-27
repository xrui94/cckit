// test_math_basic.cpp - 数学库基础功能测试
#include <cckit/math/Math.hpp>

#include <iostream>
#include <cassert>
#include <cmath>

using namespace cckit::math;

void test_constants()
{
    std::cout << "测试数学常量..." << std::endl;

    // 测试 PI
    assert(std::abs(PI<float> - 3.14159265358979323846f) < 1e-6f);
    assert(std::abs(PI<double> - 3.14159265358979323846) < 1e-14);

    // 测试 TWO_PI
    assert(std::abs(TWO_PI<float> - 2.0f * PI<float>) < 1e-6f);

    // 测试 HALF_PI
    assert(std::abs(HALF_PI<float> - 0.5f * PI<float>) < 1e-6f);

    // 测试 E
    assert(std::abs(E<float> - 2.71828182845904523536f) < 1e-6f);

    std::cout << "✅ 数学常量测试通过" << std::endl;
}

void test_vec2_operations()
{
    std::cout << "测试 Vec2 操作..." << std::endl;

    // 构造函数
    Vec2f v1(1.0f, 2.0f);
    assert(v1.x == 1.0f);
    assert(v1.y == 2.0f);

    Vec2f v2(3.0f, 4.0f);

    // 加法
    Vec2f v3 = v1 + v2;
    assert(std::abs(v3.x - 4.0f) < 1e-6f);
    assert(std::abs(v3.y - 6.0f) < 1e-6f);

    // 减法
    Vec2f v4 = v2 - v1;
    assert(std::abs(v4.x - 2.0f) < 1e-6f);
    assert(std::abs(v4.y - 2.0f) < 1e-6f);

    // 标量乘法
    Vec2f v5 = v1 * 2.0f;
    assert(std::abs(v5.x - 2.0f) < 1e-6f);
    assert(std::abs(v5.y - 4.0f) < 1e-6f);

    // 点积
    float dot = v1.dot(v2);
    assert(std::abs(dot - (1.0f * 3.0f + 2.0f * 4.0f)) < 1e-6f);

    // 长度
    float len = v1.length();
    assert(std::abs(len - std::sqrt(1.0f + 4.0f)) < 1e-6f);

    // 归一化
    Vec2f v6 = v1.normalized();
    assert(std::abs(v6.length() - 1.0f) < 1e-6f);

    std::cout << "✅ Vec2 操作测试通过" << std::endl;
}

void test_vec3_operations()
{
    std::cout << "测试 Vec3 操作..." << std::endl;

    // 构造函数
    Vec3f v1(1.0f, 2.0f, 3.0f);
    assert(v1.x == 1.0f);
    assert(v1.y == 2.0f);
    assert(v1.z == 3.0f);

    Vec3f v2(4.0f, 5.0f, 6.0f);

    // 加法
    Vec3f v3 = v1 + v2;
    assert(std::abs(v3.x - 5.0f) < 1e-6f);
    assert(std::abs(v3.y - 7.0f) < 1e-6f);
    assert(std::abs(v3.z - 9.0f) < 1e-6f);

    // 减法
    Vec3f v4 = v2 - v1;
    assert(std::abs(v4.x - 3.0f) < 1e-6f);
    assert(std::abs(v4.y - 3.0f) < 1e-6f);
    assert(std::abs(v4.z - 3.0f) < 1e-6f);

    // 点积
    float dot = v1.dot(v2);
    assert(std::abs(dot - (1.0f * 4.0f + 2.0f * 5.0f + 3.0f * 6.0f)) < 1e-6f);

    // 叉积
    Vec3f cross = v1.cross(v2);
    assert(std::abs(cross.x - (-3.0f)) < 1e-6f);
    assert(std::abs(cross.y - 6.0f) < 1e-6f);
    assert(std::abs(cross.z - (-3.0f)) < 1e-6f);

    // 长度
    float len = v1.length();
    assert(std::abs(len - std::sqrt(1.0f + 4.0f + 9.0f)) < 1e-6f);

    // 归一化
    Vec3f v5 = v1.normalized();
    assert(std::abs(v5.length() - 1.0f) < 1e-6f);

    std::cout << "✅ Vec3 操作测试通过" << std::endl;
}

void test_vec4_operations()
{
    std::cout << "测试 Vec4 操作..." << std::endl;

    // 构造函数
    Vec4f v1(1.0f, 2.0f, 3.0f, 4.0f);
    assert(v1.x == 1.0f);
    assert(v1.y == 2.0f);
    assert(v1.z == 3.0f);
    assert(v1.w == 4.0f);

    Vec4f v2(5.0f, 6.0f, 7.0f, 8.0f);

    // 加法
    Vec4f v3 = v1 + v2;
    assert(std::abs(v3.x - 6.0f) < 1e-6f);
    assert(std::abs(v3.y - 8.0f) < 1e-6f);
    assert(std::abs(v3.z - 10.0f) < 1e-6f);
    assert(std::abs(v3.w - 12.0f) < 1e-6f);

    // 点积
    float dot = v1.dot(v2);
    float expected = 1.0f * 5.0f + 2.0f * 6.0f + 3.0f * 7.0f + 4.0f * 8.0f;
    assert(std::abs(dot - expected) < 1e-6f);

    // 长度
    float len = v1.length();
    float expectedLen = std::sqrt(1.0f + 4.0f + 9.0f + 16.0f);
    assert(std::abs(len - expectedLen) < 1e-6f);

    std::cout << "✅ Vec4 操作测试通过" << std::endl;
}

void test_mat3_operations()
{
    std::cout << "测试 Mat3 操作..." << std::endl;

    // 单位矩阵
    Mat3f identity = Mat3f::identity();
    assert(std::abs(identity(0, 0) - 1.0f) < 1e-6f);
    assert(std::abs(identity(1, 1) - 1.0f) < 1e-6f);
    assert(std::abs(identity(2, 2) - 1.0f) < 1e-6f);

    // 零矩阵
    Mat3f zero = Mat3f::zero();
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            assert(std::abs(zero(i, j)) < 1e-6f);
        }
    }

    // 矩阵乘法
    Vec3f v(1.0f, 2.0f, 3.0f);
    Vec3f result = identity * v;
    assert(std::abs(result.x - 1.0f) < 1e-6f);
    assert(std::abs(result.y - 2.0f) < 1e-6f);
    assert(std::abs(result.z - 3.0f) < 1e-6f);

    // 行列式
    float det = identity.determinant();
    assert(std::abs(det - 1.0f) < 1e-6f);

    // 转置
    Mat3f transposed = identity.transpose();
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            assert(std::abs(transposed(i, j) - identity(i, j)) < 1e-6f);
        }
    }

    std::cout << "✅ Mat3 操作测试通过" << std::endl;
}

void test_mat4_operations()
{
    std::cout << "测试 Mat4 操作..." << std::endl;

    // 单位矩阵
    Mat4f identity = Mat4f::identity();
    assert(std::abs(identity(0, 0) - 1.0f) < 1e-6f);
    assert(std::abs(identity(1, 1) - 1.0f) < 1e-6f);
    assert(std::abs(identity(2, 2) - 1.0f) < 1e-6f);
    assert(std::abs(identity(3, 3) - 1.0f) < 1e-6f);

    // 零矩阵
    Mat4f zero = Mat4f::zero();
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            assert(std::abs(zero(i, j)) < 1e-6f);
        }
    }

    // 矩阵乘法
    Vec4f v(1.0f, 2.0f, 3.0f, 4.0f);
    Vec4f result = identity * v;
    assert(std::abs(result.x - 1.0f) < 1e-6f);
    assert(std::abs(result.y - 2.0f) < 1e-6f);
    assert(std::abs(result.z - 3.0f) < 1e-6f);
    assert(std::abs(result.w - 4.0f) < 1e-6f);

    // 行列式
    float det = identity.determinant();
    assert(std::abs(det - 1.0f) < 1e-6f);

    std::cout << "✅ Mat4 操作测试通过" << std::endl;
}

void test_quat_operations()
{
    std::cout << "测试 Quat 操作..." << std::endl;

    // 单位四元数
    Quatf identity = Quatf::identity();
    assert(std::abs(identity.w - 1.0f) < 1e-6f);
    assert(std::abs(identity.x) < 1e-6f);
    assert(std::abs(identity.y) < 1e-6f);
    assert(std::abs(identity.z) < 1e-6f);

    // 从角度轴创建
    Quatf rotation = Quatf::fromAxisAngle(Vec3f(0.0f, 0.0f, 1.0f), HALF_PI<float>);
    assert(std::abs(rotation.length() - 1.0f) < 1e-6f);

    // 四元数乘法
    Quatf result = rotation * identity;
    assert(std::abs(result.w - rotation.w) < 1e-6f);

    // 归一化
    Quatf q(1.0f, 2.0f, 3.0f, 4.0f);
    Quatf normalized = q.normalized();
    assert(std::abs(normalized.length() - 1.0f) < 1e-6f);

    // 共轭
    Quatf conjugated = q.conjugate();
    assert(std::abs(conjugated.x + q.x) < 1e-6f);
    assert(std::abs(conjugated.y + q.y) < 1e-6f);
    assert(std::abs(conjugated.z + q.z) < 1e-6f);
    assert(std::abs(conjugated.w - q.w) < 1e-6f);

    std::cout << "✅ Quat 操作测试通过" << std::endl;
}

void test_math_utilities()
{
    std::cout << "测试数学工具函数..." << std::endl;

    // 角度转换
    float degrees = 180.0f;
    float radians = degToRad(degrees);
    assert(std::abs(radians - PI<float>) < 1e-6f);

    float backToDegrees = radToDeg(radians);
    assert(std::abs(backToDegrees - degrees) < 1e-6f);

    // 线性插值
    float a = 0.0f;
    float b = 10.0f;
    float t = 0.5f;
    float lerpResult = lerp(a, b, t);
    assert(std::abs(lerpResult - 5.0f) < 1e-6f);

    // 夹紧
    float clamped = clamp(15.0f, 0.0f, 10.0f);
    assert(std::abs(clamped - 10.0f) < 1e-6f);

    // 最小值和最大值
    float minVal = std::min(3.0f, 7.0f);
    float maxVal = std::max(3.0f, 7.0f);
    assert(std::abs(minVal - 3.0f) < 1e-6f);
    assert(std::abs(maxVal - 7.0f) < 1e-6f);

    std::cout << "✅ 数学工具函数测试通过" << std::endl;
}

void test_tolerance()
{
    std::cout << "测试容差比较..." << std::endl;

    // 测试近似相等
    float a = 1.0f;
    float b = 1.000001f;
    assert(isEqual(a, b));

    float c = 1.0f;
    float d = 1.01f;
    assert(!isEqual(c, d));

    // 测试近似为零
    float e = 0.000001f;
    assert(isZero(e));

    float f = 0.01f;
    assert(!isZero(f));

    std::cout << "✅ 容差比较测试通过" << std::endl;
}

void test_edge_cases()
{
    std::cout << "测试边界情况..." << std::endl;

    // 零向量
    Vec3f zeroVec;
    assert(std::abs(zeroVec.length()) < 1e-6f);

    // 零向量的归一化（应该返回零向量）
    Vec3f normalizedZero = zeroVec.normalized();
    assert(std::abs(normalizedZero.length()) < 1e-6f);

    // 大数值
    Vec3f largeVec(1e6f, 1e6f, 1e6f);
    float largeLen = largeVec.length();
    assert(largeLen > 0);

    // 负数值
    Vec3f negativeVec(-1.0f, -2.0f, -3.0f);
    float negativeLen = negativeVec.length();
    assert(negativeLen > 0);

    std::cout << "✅ 边界情况测试通过" << std::endl;
}

int main()
{
    std::cout << "=== 数学库基础功能测试 ===" << std::endl;
    std::cout << std::endl;

    test_constants();
    std::cout << std::endl;

    test_vec2_operations();
    std::cout << std::endl;

    test_vec3_operations();
    std::cout << std::endl;

    test_vec4_operations();
    std::cout << std::endl;

    test_mat3_operations();
    std::cout << std::endl;

    test_mat4_operations();
    std::cout << std::endl;

    test_quat_operations();
    std::cout << std::endl;

    test_math_utilities();
    std::cout << std::endl;

    test_tolerance();
    std::cout << std::endl;

    test_edge_cases();
    std::cout << std::endl;

    std::cout << "=== 所有测试通过! ===" << std::endl;
    return 0;
}
