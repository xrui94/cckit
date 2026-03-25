// Tolerance.hpp - 容差处理定义和工具函数

#pragma once

#include <cmath>
#include <algorithm>
#include <type_traits>

namespace cckit::math {

    template<typename T>
    constexpr T getTolerance() {
        if constexpr (std::is_same_v<T, float>) {
            return static_cast<T>(1e-6);
        } else if constexpr (std::is_same_v<T, double>) {
            return static_cast<T>(1e-12);
        } else {
            return static_cast<T>(1e-10);
        }
    }

    template<typename T>
    constexpr bool isApproxEqual(T a, T b, T eps = getTolerance<T>()) {
        return std::abs(a - b) <= eps;
    }

    template<typename T>
    constexpr bool isApproxZero(T a, T eps = getTolerance<T>()) {
        return std::abs(a) <= eps;
    }

} // namespace cckit::math
