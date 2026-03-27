// include/cckit/Utils.hpp
// Utils.hpp - CCKIT 数学库工具函数，常用方法接口

#pragma once

// 首先包含配置，确保宏生效 (虽然各个子头文件也会包含，但在这里包含确保顺序)
#include "MathConfig.h"
#include "Constants.hpp" // 工具函数经常需要用到 PI 等常量
#include "types/Vec.hpp"
#include "types/Mat.hpp"
#include "types/Quat.hpp"

#include <cmath>
#include <algorithm>

namespace cckit::math
{

    /// @brief 近似相等比较（便捷别名）
    /// @param a 第一个值
    /// @param b 第二个值
    /// @param eps 容差（可选，默认使用类型默认容差）
    /// @return 是否近似相等
    template<typename T>
    constexpr bool isEqual(T a, T b, T eps = getTolerance<T>()) {
        return isApproxEqual(a, b, eps);
    }

    /// @brief 近似为零检查（便捷别名）
    /// @param a 要检查的值
    /// @param eps 容差（可选，默认使用类型默认容差）
    /// @return 是否近似为零
    template<typename T>
    constexpr bool isZero(T a, T eps = getTolerance<T>()) {
        return isApproxZero(a, eps);
    }

    // template<typename T>
    // constexpr T radians(T degrees) {
    //     return degrees * DEG_TO_RAD;
    // }

    // template<typename T>
    // constexpr T degrees(T radians) {
    //     return radians * RAD_TO_DEG;
    // }

    template<typename T>
    inline T toRadians(T angle) {
        return angle * DEG_TO_RAD<T>;
    }

    template<typename T>
    inline T toDegrees(T angle) {
        return angle * RAD_TO_DEG<T>;
    }

    template<typename T>
    inline T degToRad(T angle) {
        return angle * DEG_TO_RAD<T>;
    }

    template<typename T>
    inline T radToDeg(T angle) {
        return angle * RAD_TO_DEG<T>;
    }

    template<typename T>
    constexpr T clamp(T v, T lo, T hi) {
        // 确保 lo <= hi ? 或者完全信任用户？
        // std::clamp (C++17) 行为是：如果 lo > hi 返回 hi。
        // 这里直接委托给 std::clamp 如果可用，否则手动实现
        #if __cplusplus >= 201703L
            return std::clamp(v, lo, hi);
        #else
            return std::min(hi, std::max(lo, v));
        #endif
    }

    // 添加常用的 abs, sqrt 包装 (可选，为了方便统一命名空间)
    template<typename T>
    constexpr T abs(T v) {
        if constexpr (std::is_floating_point_v<T>) {
            return static_cast<T>(std::fabs(static_cast<double>(v)));
        } else {
            return v < T(0) ? -v : v;
        }
    }

    /// @brief 线性插值 (Linear Interpolation)
    /// @param a 起点
    /// @param b 终点
    /// @param t 插值因子 (0.0 = a, 1.0 = b)
    template<typename T, typename U>
    constexpr T lerp(const T& a, const T& b, U t) {
        return static_cast<T>(a + (b - a) * t);
    }

    /// @brief 平滑步进 (Smoothstep)
    template<typename T>
    constexpr T smoothstep(T edge0, T edge1, T x) {
        T t = clamp((x - edge0) / (edge1 - edge0), T(0), T(1));
        return t * t * (T(3) - T(2) * t);
    }
    
    /// @brief 更平滑的步进 (Smootherstep, Ken Perlin 提出)
    template<typename T>
    constexpr T smootherstep(T edge0, T edge1, T x) {
        T t = clamp((x - edge0) / (edge1 - edge0), T(0), T(1));
        return t * t * t * (t * (t * T(6) - T(15)) + T(10));
    }

} // namespace cckit::math
