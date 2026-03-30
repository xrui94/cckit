// cartographic.h - Geodetic coordinates C API
#pragma once

#include "cckit/export.h"
#include "cartesian3.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// Cartographic - Geodetic coordinates (longitude, latitude, height)
// ========================================

/**
 * @brief Cartographic struct - Geodetic coordinates
 */
typedef struct {
    double longitude;  // Longitude in radians
    double latitude;   // Latitude in radians
    double height;     // Height in meters
} cckit_cartographic_t;

/**
 * @brief Create Cartographic from radians
 */
CCKIT_API cckit_cartographic_t cckit_cartographic_from_radians(double longitude, double latitude, double height);

/**
 * @brief Create Cartographic from degrees
 */
CCKIT_API cckit_cartographic_t cckit_cartographic_from_degrees(double longitude, double latitude, double height);

/**
 * @brief Convert Cartesian3 to Cartographic
 */
CCKIT_API cckit_cartographic_t cckit_cartographic_from_cartesian(cckit_cartesian3_t cartesian);

/**
 * @brief Convert Cartographic to Cartesian3
 */
CCKIT_API cckit_cartesian3_t cckit_cartographic_to_cartesian(cckit_cartographic_t cartographic);

#ifdef __cplusplus
}
#endif
