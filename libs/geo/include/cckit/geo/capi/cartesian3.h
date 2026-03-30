// cartesian3.h - Cartesian coordinates C API
#pragma once

#include "cckit/export.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ========================================
// Cartesian3 - Cartesian coordinates (X, Y, Z)
// ========================================

/**
 * @brief Cartesian3 struct - Earth-Centered Earth-Fixed Cartesian coordinates
 */
typedef struct {
    double x;
    double y;
    double z;
} cckit_cartesian3_t;

/**
 * @brief Create Cartesian3
 */
CCKIT_API cckit_cartesian3_t cckit_cartesian3_create(double x, double y, double z);

/**
 * @brief Create Cartesian3 from degrees
 */
CCKIT_API cckit_cartesian3_t cckit_cartesian3_from_degrees(double longitude, double latitude, double height);

/**
 * @brief Create Cartesian3 from radians
 */
CCKIT_API cckit_cartesian3_t cckit_cartesian3_from_radians(double longitude, double latitude, double height);

/**
 * @brief Convert to degrees
 */
CCKIT_API void cckit_cartesian3_to_degrees(cckit_cartesian3_t cartesian, double* longitude, double* latitude, double* height);

/**
 * @brief Convert to radians
 */
CCKIT_API void cckit_cartesian3_to_radians(cckit_cartesian3_t cartesian, double* longitude, double* latitude, double* height);

/**
 * @brief Vector addition
 */
CCKIT_API cckit_cartesian3_t cckit_cartesian3_add(cckit_cartesian3_t left, cckit_cartesian3_t right);

/**
 * @brief Vector subtraction
 */
CCKIT_API cckit_cartesian3_t cckit_cartesian3_subtract(cckit_cartesian3_t left, cckit_cartesian3_t right);

/**
 * @brief Dot product
 */
CCKIT_API double cckit_cartesian3_dot(cckit_cartesian3_t left, cckit_cartesian3_t right);

/**
 * @brief Cross product
 */
CCKIT_API cckit_cartesian3_t cckit_cartesian3_cross(cckit_cartesian3_t left, cckit_cartesian3_t right);

/**
 * @brief Vector magnitude
 */
CCKIT_API double cckit_cartesian3_magnitude(cckit_cartesian3_t cartesian);

/**
 * @brief Vector magnitude squared
 */
CCKIT_API double cckit_cartesian3_magnitude_squared(cckit_cartesian3_t cartesian);

/**
 * @brief Normalize
 */
CCKIT_API cckit_cartesian3_t cckit_cartesian3_normalize(cckit_cartesian3_t cartesian);

/**
 * @brief Scalar multiplication
 */
CCKIT_API cckit_cartesian3_t cckit_cartesian3_multiply_by_scalar(cckit_cartesian3_t cartesian, double scalar);

/**
 * @brief Scalar division
 */
CCKIT_API cckit_cartesian3_t cckit_cartesian3_divide_by_scalar(cckit_cartesian3_t cartesian, double scalar);

/**
 * @brief Component-wise multiplication
 */
CCKIT_API cckit_cartesian3_t cckit_cartesian3_multiply_components(cckit_cartesian3_t left, cckit_cartesian3_t right);

/**
 * @brief Scale to geodetic surface
 */
CCKIT_API cckit_cartesian3_t cckit_cartesian3_scale_to_geodetic_surface(
    cckit_cartesian3_t cartesian,
    cckit_cartesian3_t oneOverRadii,
    cckit_cartesian3_t oneOverRadiiSquared,
    double centerToleranceSquared);

#ifdef __cplusplus
}
#endif
