// geo_math.h - Math constants C API
#pragma once

#include "cckit/export.h"

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// Math constants
// ========================================

/**
 * @brief Radians to degrees
 */
CCKIT_GEO_API double cckit_geo_radians_to_degrees(double radians);

/**
 * @brief Degrees to radians
 */
CCKIT_GEO_API double cckit_geo_degrees_to_radians(double degrees);

#ifdef __cplusplus
}
#endif
