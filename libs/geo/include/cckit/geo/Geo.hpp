// Geo.hpp - Geographic/GIS utilities main header
#pragma once

// Include C++ headers FIRST (before C API header)
#include <cmath>
#include <string>
#include <sstream>

// C API
#include "cckit/geo/capi/geo.h"

// C++ types
#include "cckit/geo/types/Cartesian3.hpp"
#include "cckit/geo/types/Cartographic.hpp"

namespace cckit::geo
{

    // ========================================
    // Math constants
    // ========================================

    constexpr double PI = 3.14159265358979323846;
    constexpr double PI_OVER_180 = PI / 180.0;
    constexpr double ONE_OVER_PI = 1.0 / PI;
    constexpr double DEGREES_PER_RADIAN = 180.0 / PI;
    constexpr double RADIANS_PER_DEGREE = PI / 180.0;

    // ========================================
    // Angle conversion (inline wrappers)
    // ========================================

    /**
     * @brief Radians to degrees
     */
    inline double radiansToDegrees(double radians) {
        return radians * DEGREES_PER_RADIAN;
    }

    /**
     * @brief Degrees to radians
     */
    inline double degreesToRadians(double degrees) {
        return degrees * RADIANS_PER_DEGREE;
    }

} // namespace cckit::geo
