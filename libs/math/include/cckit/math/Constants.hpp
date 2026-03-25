// include/cckit/Constants.hpp
// Constants.hpp - CCKIT 数学库常量定义

#pragma once

// 首先包含配置，确保宏生效 (虽然各个子头文件也会包含，但在这里包含确保顺序)
#include "MathConfig.h"

#include <type_traits>

namespace cckit::math
{

    // ========================================
    // 数学常量(使用变量模板，C++17 风格)
    // 使用变量模板 (Variable Templates) 是最现代、最高效的做法
    // 隐式 inline，没有链接错误风险，且支持 constexpr
    // ========================================
    template<typename T>
    inline constexpr T PI = static_cast<T>(3.14159265358979323846);

    template<typename T>
    inline constexpr T TWO_PI = static_cast<T>(2.0) * PI<T>;

    template<typename T>
    inline constexpr T HALF_PI = static_cast<T>(0.5) * PI<T>;

    // 特殊常量：欧拉数 e
    template<typename T>
    inline constexpr T E = static_cast<T>(2.71828182845904523536);

    template<typename T>
    inline constexpr T EPSILON = static_cast<T>(1e-6); // 常用浮点误差容限

    template<typename T>
    inline constexpr T DEG_TO_RAD = PI<T> / static_cast<T>(180.0);

    template<typename T>
    inline constexpr T RAD_TO_DEG = static_cast<T>(180.0) / PI<T>;

    // template<typename T>
    // struct Constants {
    //     static constexpr T PI = static_cast<T>(3.14159265358979323846);
    //     static constexpr T TWO_PI = static_cast<T>(2.0) * PI;
    //     static constexpr T HALF_PI = static_cast<T>(0.5) * PI;
    //     static constexpr T DEG_TO_RAD = PI / static_cast<T>(180.0);
    //     static constexpr T RAD_TO_DEG = static_cast<T>(180.0) / PI;
    // };

} // namespace cckit::math
