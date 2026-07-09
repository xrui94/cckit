// test_math_basic.cpp - 数学库基础功能测试
#include <cckit/math/Math.hpp>

#include <gtest/gtest.h>
#include <cmath>

using namespace cckit::math;

TEST(MathTest, Constants)
{
    EXPECT_NEAR(PI<float>, 3.14159265358979323846f, 1e-6f);
    EXPECT_NEAR(PI<double>, 3.14159265358979323846, 1e-14);
    EXPECT_NEAR(TWO_PI<float>, 2.0f * PI<float>, 1e-6f);
    EXPECT_NEAR(HALF_PI<float>, 0.5f * PI<float>, 1e-6f);
    EXPECT_NEAR(E<float>, 2.71828182845904523536f, 1e-6f);
}

TEST(MathTest, Vec2Operations)
{
    Vec2f v1(1.0f, 2.0f);
    EXPECT_EQ(v1.x, 1.0f);
    EXPECT_EQ(v1.y, 2.0f);

    Vec2f v2(3.0f, 4.0f);
    Vec2f v3 = v1 + v2;
    EXPECT_NEAR(v3.x, 4.0f, 1e-6f);
    EXPECT_NEAR(v3.y, 6.0f, 1e-6f);

    Vec2f v4 = v2 - v1;
    EXPECT_NEAR(v4.x, 2.0f, 1e-6f);
    EXPECT_NEAR(v4.y, 2.0f, 1e-6f);

    float dot = v1 * v2;
    EXPECT_NEAR(dot, 1.0f * 3.0f + 2.0f * 4.0f, 1e-6f);

    Vec2f v5 = v1 * 2.0f;
    EXPECT_NEAR(v5.x, 2.0f, 1e-6f);
    EXPECT_NEAR(v5.y, 4.0f, 1e-6f);

    EXPECT_NEAR(v1.length(), std::sqrt(5.0f), 1e-6f);
    EXPECT_NEAR(v1.lengthSquared(), 5.0f, 1e-6f);

    Vec2f normalized = v1.normalized();
    EXPECT_NEAR(normalized.length(), 1.0f, 1e-5f);

    EXPECT_TRUE(v1 == Vec2f(1.0f, 2.0f));
    EXPECT_FALSE(v1 == v2);
}

TEST(MathTest, Vec3Operations)
{
    Vec3f v1(1.0f, 2.0f, 3.0f);
    Vec3f v2(4.0f, 5.0f, 6.0f);

    Vec3f sum = v1 + v2;
    EXPECT_NEAR(sum.x, 5.0f, 1e-6f);
    EXPECT_NEAR(sum.y, 7.0f, 1e-6f);
    EXPECT_NEAR(sum.z, 9.0f, 1e-6f);

    Vec3f diff = v2 - v1;
    EXPECT_NEAR(diff.x, 3.0f, 1e-6f);
    EXPECT_NEAR(diff.y, 3.0f, 1e-6f);
    EXPECT_NEAR(diff.z, 3.0f, 1e-6f);

    float dotProduct = v1 * v2;
    EXPECT_NEAR(dotProduct, 32.0f, 1e-6f);

    Vec3f cross = v1.cross(v2);
    EXPECT_NEAR(cross.x, -3.0f, 1e-6f);
    EXPECT_NEAR(cross.y, 6.0f, 1e-6f);
    EXPECT_NEAR(cross.z, -3.0f, 1e-6f);

    EXPECT_NEAR(v1.length(), std::sqrt(14.0f), 1e-6f);

    Vec3f normalized = v1.normalized();
    EXPECT_NEAR(normalized.length(), 1.0f, 1e-5f);

    EXPECT_TRUE(v1 == Vec3f(1.0f, 2.0f, 3.0f));
}

TEST(MathTest, Vec3Transform)
{
    Vec3f v(1.0f, 0.0f, 0.0f);

    Vec3f rotated90 = v.rotated(HALF_PI<float>, Vec3f::unitZ());
    EXPECT_NEAR(rotated90.x, 0.0f, 1e-5f);
    EXPECT_NEAR(rotated90.y, 1.0f, 1e-5f);
    EXPECT_NEAR(rotated90.z, 0.0f, 1e-5f);

    Vec3f scaled = v.scaled(2.0f);
    EXPECT_NEAR(scaled.x, 2.0f, 1e-6f);
    EXPECT_NEAR(scaled.y, 0.0f, 1e-6f);

    Vec3f scaledPerAxis = v.scaled(Vec3f(2.0f, 3.0f, 4.0f));
    EXPECT_NEAR(scaledPerAxis.x, 2.0f, 1e-6f);
    EXPECT_NEAR(scaledPerAxis.y, 0.0f, 1e-6f);
    EXPECT_NEAR(scaledPerAxis.z, 0.0f, 1e-6f);

    Vec3f scaledPivot = v.scaled(Vec3f(0.0f, 0.0f, 0.0f), 2.0f);
    EXPECT_NEAR(scaledPivot.x, 2.0f, 1e-6f);

    Vec3f translated = v.translated(Vec3f(1.0f, 2.0f, 3.0f));
    EXPECT_NEAR(translated.x, 2.0f, 1e-6f);
    EXPECT_NEAR(translated.y, 2.0f, 1e-6f);
    EXPECT_NEAR(translated.z, 3.0f, 1e-6f);

    Mat4f m = Mat4f::identity();
    Vec3f transformed = v.transformed(m);
    EXPECT_NEAR(transformed.x, 1.0f, 1e-5f);
    EXPECT_NEAR(transformed.y, 0.0f, 1e-5f);
    EXPECT_NEAR(transformed.z, 0.0f, 1e-5f);
}

TEST(MathTest, Vec4Operations)
{
    Vec4f v1(1.0f, 2.0f, 3.0f, 4.0f);
    Vec4f v2(5.0f, 6.0f, 7.0f, 8.0f);

    Vec4f sum = v1 + v2;
    EXPECT_NEAR(sum.x, 6.0f, 1e-6f);
    EXPECT_NEAR(sum.y, 8.0f, 1e-6f);
    EXPECT_NEAR(sum.z, 10.0f, 1e-6f);
    EXPECT_NEAR(sum.w, 12.0f, 1e-6f);

    EXPECT_NEAR(v1.length(), std::sqrt(30.0f), 1e-6f);
}

TEST(MathTest, Mat3Operations)
{
    Mat3f identity = Mat3f::identity();
    EXPECT_TRUE(identity.isIdentity());

    Mat3f rotated = Mat3f::fromRotation(HALF_PI<float>);
    Vec3f v(1.0f, 0.0f, 0.0f);
    Vec3f result = rotated.transformVector(v);
    EXPECT_NEAR(result.x, 0.0f, 1e-5f);
    EXPECT_NEAR(result.y, 1.0f, 1e-5f);
    EXPECT_NEAR(result.z, 0.0f, 1e-5f);
}

TEST(MathTest, Mat4Operations)
{
    Mat4f identity = Mat4f::identity();
    EXPECT_TRUE(identity.isIdentity());

    Mat4f translation = Mat4f::fromTranslation(Vec3f(1.0f, 2.0f, 3.0f));
    Vec3f v(0.0f, 0.0f, 0.0f);
    Vec3f result = translation.transformPoint(v);
    EXPECT_NEAR(result.x, 1.0f, 1e-5f);
    EXPECT_NEAR(result.y, 2.0f, 1e-5f);
    EXPECT_NEAR(result.z, 3.0f, 1e-5f);

    Mat4f a = Mat4f::fromTranslation(Vec3f(1.0f, 0.0f, 0.0f));
    Mat4f b = Mat4f::fromTranslation(Vec3f(0.0f, 2.0f, 0.0f));
    Mat4f c = a * b;
    Vec3f r = c.transformPoint(Vec3f(0.0f, 0.0f, 0.0f));
    EXPECT_NEAR(r.x, 1.0f, 1e-5f);
    EXPECT_NEAR(r.y, 2.0f, 1e-5f);
    EXPECT_NEAR(r.z, 0.0f, 1e-5f);

    Mat4f transposed = a.transpose();
    transposed(3, 0) = 0.0f;  // 恢复标准仿射矩阵格式
    transposed(3, 1) = 0.0f;
    transposed(3, 2) = 0.0f;
    transposed(3, 3) = 1.0f;
    EXPECT_TRUE(transposed.isIdentity());
}

TEST(MathTest, QuatOperations)
{
    Quatf identity = Quatf::identity();
    Vec3f v(1.0f, 0.0f, 0.0f);
    Vec3f rotated = identity.rotate(v);
    EXPECT_NEAR(rotated.x, 1.0f, 1e-5f);
    EXPECT_NEAR(rotated.y, 0.0f, 1e-5f);
    EXPECT_NEAR(rotated.z, 0.0f, 1e-5f);

    Quatf qZ90(Vec3f::unitZ(), HALF_PI<float>);
    Vec3f rotated90 = qZ90.rotate(v);
    EXPECT_NEAR(rotated90.x, 0.0f, 1e-5f);
    EXPECT_NEAR(rotated90.y, 1.0f, 1e-5f);
    EXPECT_NEAR(rotated90.z, 0.0f, 1e-5f);

    Quatf normalized = qZ90.normalized();
    EXPECT_NEAR(normalized.length(), 1.0f, 1e-5f);
}

TEST(MathTest, Utils)
{
    EXPECT_NEAR(toDegrees(PI<float>), 180.0f, 1e-4f);
    EXPECT_NEAR(toRadians(180.0f), PI<float>, 1e-4f);
    EXPECT_NEAR(clamp(0.5f, 0.0f, 1.0f), 0.5f, 1e-6f);
    EXPECT_NEAR(clamp(2.0f, 0.0f, 1.0f), 1.0f, 1e-6f);
    EXPECT_NEAR(clamp(-1.0f, 0.0f, 1.0f), 0.0f, 1e-6f);
    EXPECT_NEAR(lerp(0.0f, 10.0f, 0.5f), 5.0f, 1e-6f);
}

TEST(MathTest, Tolerance)
{
    EXPECT_TRUE(isApproxEqual(1.0f, 1.0000001f, 1e-6f));
    EXPECT_FALSE(isApproxEqual(1.0f, 1.001f, 1e-6f));
    EXPECT_TRUE(isApproxZero(0.0f, 1e-6f));
    EXPECT_FALSE(isApproxZero(0.001f, 1e-6f));
}

TEST(MathTest, EdgeCases)
{
    Vec3f zero = Vec3f::zero();
    Vec3f normalized = zero.normalized();
    EXPECT_NEAR(normalized.x, 0.0f, 1e-6f);

    Vec3f v(1.0f, 0.0f, 0.0f);
    Vec3f div = v / 0.0f;
    EXPECT_TRUE(std::isnan(div.x));

    Mat4f identity = Mat4f::identity();
    Mat4f inv = identity.inverse();
    EXPECT_TRUE(inv.isIdentity());
}
