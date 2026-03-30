// ellipsoid.h - Ellipsoid model C API
#pragma once

#include "cckit/export.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// Ellipsoid - Ellipsoid model
// ========================================

/**
 * @brief Ellipsoid struct - Ellipsoid parameters
 */
typedef struct {
    double x_radius;           // X radius (equatorial)
    double y_radius;           // Y radius (equatorial)
    double z_radius;           // Z radius (polar)
    double x_radius_squared;
    double y_radius_squared;
    double z_radius_squared;
    double one_over_x_radius;
    double one_over_y_radius;
    double one_over_z_radius;
    double one_over_x_radius_squared;
    double one_over_y_radius_squared;
    double one_over_z_radius_squared;
} cckit_ellipsoid_t;

/**
 * @brief Get WGS84 ellipsoid
 */
CCKIT_API cckit_ellipsoid_t cckit_ellipsoid_wgs84(void);

/**
 * @brief Create custom ellipsoid
 */
CCKIT_API cckit_ellipsoid_t cckit_ellipsoid_create(double x_radius, double y_radius, double z_radius);

#ifdef __cplusplus
}
#endif
