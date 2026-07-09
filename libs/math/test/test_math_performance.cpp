// test_math_performance.cpp - 数学库性能测试
#include <cckit/math/Math.hpp>

#include <gtest/gtest.h>
#include <chrono>

using namespace cckit::math;

TEST(MathPerformance, Vec3Operations)
{
    const int count = 1000000;
    std::vector<Vec3f> vecs(count, Vec3f(1.0f, 2.0f, 3.0f));

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i < count; ++i)
        auto sum = vecs[i] + vecs[i - 1];
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "执行 " << count << " 次 Vec3 加法耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次操作耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    float dotSum = 0.0f;
    for (int i = 1; i < count; ++i)
        dotSum += vecs[i] * vecs[i - 1];
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "执行 " << count << " 次 Vec3 点积耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "点积总和: " << dotSum << " (用于防止优化)" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (auto& v : vecs)
        v = v.normalized();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "执行 " << count << " 次 Vec3 归一化耗时: " << duration.count() << " ms" << std::endl;
    SUCCEED();
}

TEST(MathPerformance, Mat4Operations)
{
    const int count = 100000;
    std::vector<Mat4f> mats(count, Mat4f::fromRotation(0.1f, Vec3f(0.0f, 0.0f, 1.0f)));

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i < count; ++i)
        auto result = mats[i] * mats[i - 1];
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "执行 " << count << " 次 Mat4 乘法耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次操作耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;

    Vec3f v(1.0f, 2.0f, 3.0f);
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; ++i)
        auto result = mats[i] * v;
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "执行 " << count << " 次 Mat4 向量乘法耗时: " << duration.count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    float detSum = 0.0f;
    for (int i = 0; i < count; ++i)
        detSum += mats[i].determinant();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "执行 " << count << " 次 Mat4 行列式计算耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "行列式总和: " << detSum << " (用于防止优化)" << std::endl;
    SUCCEED();
}

TEST(MathPerformance, QuatOperations)
{
    const int count = 100000;
    std::vector<Quatf> quats(count, Quatf(Vec3f::unitZ(), 0.1f));

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i < count; ++i)
        auto result = quats[i] * quats[i - 1];
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "执行 " << count << " 次四元数乘法耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次操作耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (auto& q : quats)
        q = q.normalized();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "执行 " << count << " 次四元数归一化耗时: " << duration.count() << " ms" << std::endl;

    Vec3f v(1.0f, 0.0f, 0.0f);
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; ++i)
        auto result = quats[i].rotate(v);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "执行 " << count << " 次四元数向量旋转耗时: " << duration.count() << " ms" << std::endl;
    SUCCEED();
}

TEST(MathPerformance, Utils)
{
    const int count = 500000;

    auto start = std::chrono::high_resolution_clock::now();
    float lerpSum = 0.0f;
    for (int i = 0; i < count; ++i)
        lerpSum += lerp(0.0f, 100.0f, 0.5f);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "执行 " << count << " 次线性插值耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "插值总和: " << lerpSum << " (用于防止优化)" << std::endl;
    SUCCEED();
}

TEST(MathPerformance, Combined)
{
    const int count = 10000;
    std::vector<Mat4f> mats(count, Mat4f::fromRotation(0.01f, Vec3f(0.0f, 1.0f, 0.0f)));
    Vec3f v(1.0f, 2.0f, 3.0f);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; ++i)
        auto result = mats[i].transformPoint(v);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "执行 " << count << " 次矩阵变换耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次变换耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;
    SUCCEED();
}
