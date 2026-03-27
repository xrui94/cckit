// test_math_performance.cpp - 数学库性能测试
#include <cckit/math/Math.hpp>

#include <iostream>
#include <chrono>
#include <vector>

using namespace cckit::math;

void test_vector_operations_performance()
{
    std::cout << "测试向量操作性能..." << std::endl;

    const int count = 1000000;

    // Vec3 加法性能
    std::vector<Vec3f> vecs1, vecs2;
    for (int i = 0; i < count; ++i)
    {
        vecs1.push_back(Vec3f(i, i + 1, i + 2));
        vecs2.push_back(Vec3f(i + 3, i + 4, i + 5));
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; ++i)
    {
        Vec3f result = vecs1[i] + vecs2[i];
        (void)result;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "执行 " << count << " 次 Vec3 加法耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次操作耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;

    // Vec3 点积性能
    start = std::chrono::high_resolution_clock::now();

    float sum = 0;
    for (int i = 0; i < count; ++i)
    {
        sum += vecs1[i].dot(vecs2[i]);
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "执行 " << count << " 次 Vec3 点积耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "点积总和: " << sum << " (用于防止优化)" << std::endl;

    // Vec3 归一化性能
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; ++i)
    {
        Vec3f normalized = vecs1[i].normalized();
        (void)normalized;
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "执行 " << count << " 次 Vec3 归一化耗时: " << duration.count() << " ms" << std::endl;

    std::cout << "✅ 向量操作性能测试通过" << std::endl;
}

void test_matrix_operations_performance()
{
    std::cout << "测试矩阵操作性能..." << std::endl;

    const int count = 100000;

    // Mat4 乘法性能
    std::vector<Mat4f> mats;
    for (int i = 0; i < count; ++i)
    {
        mats.push_back(Mat4f::identity());
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count - 1; ++i)
    {
        Mat4f result = mats[i] * mats[i + 1];
        (void)result;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "执行 " << count << " 次 Mat4 乘法耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次操作耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;

    // Mat4 向量乘法性能
    std::vector<Vec4f> vecs;
    for (int i = 0; i < count; ++i)
    {
        vecs.push_back(Vec4f(i, i + 1, i + 2, i + 3));
    }

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; ++i)
    {
        Vec4f result = mats[i] * vecs[i];
        (void)result;
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "执行 " << count << " 次 Mat4 向量乘法耗时: " << duration.count() << " ms" << std::endl;

    // Mat4 行列式性能
    start = std::chrono::high_resolution_clock::now();

    float sum = 0;
    for (const auto& mat : mats)
    {
        sum += mat.determinant();
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "执行 " << count << " 次 Mat4 行列式计算耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "行列式总和: " << sum << " (用于防止优化)" << std::endl;

    std::cout << "✅ 矩阵操作性能测试通过" << std::endl;
}

void test_quaternion_operations_performance()
{
    std::cout << "测试四元数操作性能..." << std::endl;

    const int count = 100000;

    // 四元数乘法性能
    std::vector<Quatf> quats;
    for (int i = 0; i < count; ++i)
    {
        quats.push_back(Quatf::fromAxisAngle(Vec3f(0, 0, 1), i * 0.01f));
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count - 1; ++i)
    {
        Quatf result = quats[i] * quats[i + 1];
        (void)result;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "执行 " << count << " 次四元数乘法耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次操作耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;

    // 四元数归一化性能
    start = std::chrono::high_resolution_clock::now();

    for (const auto& quat : quats)
    {
        Quatf normalized = quat.normalized();
        (void)normalized;
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "执行 " << count << " 次四元数归一化耗时: " << duration.count() << " ms" << std::endl;

    // 四元数向量旋转性能
    std::vector<Vec3f> vecs;
    for (int i = 0; i < count; ++i)
    {
        vecs.push_back(Vec3f(i, i + 1, i + 2));
    }

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; ++i)
    {
        Vec3f rotated = quats[i].rotate(vecs[i]);
        (void)rotated;
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "执行 " << count << " 次四元数向量旋转耗时: " << duration.count() << " ms" << std::endl;

    std::cout << "✅ 四元数操作性能测试通过" << std::endl;
}

void test_math_functions_performance()
{
    std::cout << "测试数学函数性能..." << std::endl;

    const int count = 1000000;

    // 线性插值性能
    std::vector<float> values;
    for (int i = 0; i < count; ++i)
    {
        values.push_back(static_cast<float>(i));
    }

    auto start = std::chrono::high_resolution_clock::now();

    float sum = 0;
    for (int i = 0; i < count / 2; ++i)
    {
        float result = lerp(values[i], values[i + 1], 0.5f);
        sum += result;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "执行 " << count / 2 << " 次线性插值耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "插值总和: " << sum << " (用于防止优化)" << std::endl;

    // 角度转换性能
    start = std::chrono::high_resolution_clock::now();

    sum = 0;
    for (const auto& val : values)
    {
        float rad = cckit::math::degToRad(val);
        float deg = cckit::math::radToDeg(rad);
        sum += deg;
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "执行 " << count << " 次角度转换耗时: " << duration.count() << " ms" << std::endl;

    // 夹紧函数性能
    start = std::chrono::high_resolution_clock::now();

    sum = 0;
    for (const auto& val : values)
    {
        float clamped = clamp(val, 0.0f, 100.0f);
        sum += clamped;
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "执行 " << count << " 次夹紧操作耗时: " << duration.count() << " ms" << std::endl;

    std::cout << "✅ 数学函数性能测试通过" << std::endl;
}

void test_combined_operations_performance()
{
    std::cout << "测试组合操作性能..." << std::endl;

    const int count = 10000;

    // 模拟变换矩阵链
    std::vector<Mat4f> transforms;
    for (int i = 0; i < count; ++i)
    {
        Mat4f translation = Mat4f::fromTranslation(Vec3f(i, i + 1, i + 2));
        Mat4f rotation = Mat4f::fromRotation(Quatf::fromAxisAngle(Vec3f(0, 0, 1), i * 0.01f));
        Mat4f scale = Mat4f::fromScale(Vec3f(1.0f, 1.0f, 1.0f));
        transforms.push_back(translation * rotation * scale);
    }

    // 应用变换到向量
    std::vector<Vec4f> points;
    for (int i = 0; i < count; ++i)
    {
        points.push_back(Vec4f(i, i + 1, i + 2, 1.0f));
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count; ++i)
    {
        Vec4f transformed = transforms[i] * points[i];
        (void)transformed;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "执行 " << count << " 次矩阵变换耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每次变换耗时: " << (duration.count() * 1000.0 / count) << " μs" << std::endl;

    std::cout << "✅ 组合操作性能测试通过" << std::endl;
}

int main()
{
    std::cout << "=== 数学库性能测试 ===" << std::endl;
    std::cout << std::endl;

    test_vector_operations_performance();
    std::cout << std::endl;

    test_matrix_operations_performance();
    std::cout << std::endl;

    test_quaternion_operations_performance();
    std::cout << std::endl;

    test_math_functions_performance();
    std::cout << std::endl;

    test_combined_operations_performance();
    std::cout << std::endl;

    std::cout << "=== 所有性能测试完成! ===" << std::endl;
    return 0;
}
