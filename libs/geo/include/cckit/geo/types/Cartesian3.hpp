// Cartesian3.hpp - Cartesian coordinates C++ interface
//
// 3D point in Earth-Centered, Earth-Fixed (ECEF) coordinate system
// Consistent with Cesium's Cartesian3 concept
//
// Design principles:
// 1. Maintain API compatibility with existing Cartesian3.h/.cpp
// 2. Provide modern C++ style operator overloading
// 3. Support conversion to/from Cartographic

#pragma once

// Include C++ headers FIRST (before C API header)
#include <cmath>
#include <string>
#include <sstream>

// C API
#include "cckit/geo/capi/geo.h"

namespace cckit::geo
{

    // ========================================
    // Cartesian3 - Cartesian coordinates
    // ========================================

    /**
     * @brief Earth-Centered Earth-Fixed Cartesian coordinates (X, Y, Z)
     *
     * Coordinate system definition:
     * - X axis points to intersection of prime meridian and equator
     * - Y axis points to intersection of 90E meridian and equator
     * - Z axis points to North Pole
     */
    class Cartesian3
    {
    public:
        double x;
        double y;
        double z;

        // ========================================
        // Constructors
        // ========================================

        Cartesian3() : x(0.0), y(0.0), z(0.0) {}

        Cartesian3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

        // Construct from C struct
        explicit Cartesian3(const cckit_cartesian3_t& c) : x(c.x), y(c.y), z(c.z) {}

        // ========================================
        // Static factory methods (API compatible)
        // ========================================

        /**
         * @brief Create from degrees
         */
        static Cartesian3 fromDegree(double lon, double lat, double height = 0);

        /**
         * @brief Create from radians
         */
        static Cartesian3 fromRadians(double longitude, double latitude, double height = 0);

        // ========================================
        // Static methods (API compatible)
        // ========================================

        static Cartesian3 add(Cartesian3 left, Cartesian3 right);
        static Cartesian3 subtract(Cartesian3 left, Cartesian3 right);
        static double dot(Cartesian3 left, Cartesian3 right);
        static Cartesian3 cross(Cartesian3 left, Cartesian3 right);
        static double magnitudeSquared(Cartesian3 cartesian);
        static double magnitude(Cartesian3 cartesian);
        static Cartesian3 normalize(Cartesian3 cartesian);
        static Cartesian3 multiplyByScalar(Cartesian3 cartesian, double scalar);
        static Cartesian3 divideByScalar(Cartesian3 cartesian, double scalar);
        static Cartesian3 multiplyComponents(Cartesian3 left, Cartesian3 right);

        /**
         * @brief Scale to geodetic surface
         */
        static Cartesian3 scaleToGeodeticSurface(
            Cartesian3 cartesian,
            Cartesian3 oneOverRadii,
            Cartesian3 oneOverRadiiSquared,
            double centerToleranceSquared);

        /**
         * @brief Compute horizon culling point
         */
        static Cartesian3 computeHorizonCullingPointFromVertices(
            Cartesian3 directionToPoint,
            double* vertices,
            int elementSize,
            int stride,
            Cartesian3 center);

        static Cartesian3 magnitudeToPoint(Cartesian3 scaledSpaceDirectionToPoint, double resultMagnitude);
        static Cartesian3 computeScaledSpaceDirectionToPoint(Cartesian3 directionToPoint);
        static Cartesian3 transformPositionToScaledSpace(Cartesian3 directionToPoint);
        static Cartesian3 transformScaledSpaceToPosition(Cartesian3 directionToPoint);
        static double computeMagnitude(Cartesian3 position, Cartesian3 scaledSpaceDirectionToPoint);

        // ========================================
        // Member methods
        // ========================================

        /**
         * @brief Convert to radians
         */
        void toRadians(double& longitude, double& latitude, double& height);

        /**
         * @brief Convert to degrees
         */
        void toDegrees(double& longitude, double& latitude, double& height);

        // ========================================
        // Operator overloading (modern C++ style)
        // ========================================

        Cartesian3 operator+(const Cartesian3& other) const {
            return Cartesian3(x + other.x, y + other.y, z + other.z);
        }

        Cartesian3 operator-(const Cartesian3& other) const {
            return Cartesian3(x - other.x, y - other.y, z - other.z);
        }

        Cartesian3 operator*(double scalar) const {
            return Cartesian3(x * scalar, y * scalar, z * scalar);
        }

        Cartesian3 operator/(double scalar) const {
            return Cartesian3(x / scalar, y / scalar, z / scalar);
        }

        Cartesian3 operator-() const {
            return Cartesian3(-x, -y, -z);
        }

        bool operator==(const Cartesian3& other) const {
            return x == other.x && y == other.y && z == other.z;
        }

        bool operator!=(const Cartesian3& other) const {
            return !(*this == other);
        }

        // ========================================
        // Conversion
        // ========================================

        /**
         * @brief Convert to C struct
         */
        cckit_cartesian3_t toC() const {
            cckit_cartesian3_t c;
            c.x = x;
            c.y = y;
            c.z = z;
            return c;
        }

        /**
         * @brief Convert from C struct
         */
        static Cartesian3 fromC(const cckit_cartesian3_t& c) {
            return Cartesian3(c.x, c.y, c.z);
        }

        /**
         * @brief Convert to string
         */
        std::string toString() const {
            std::ostringstream ss;
            ss << "(" << x << ", " << y << ", " << z << ")";
            return ss.str();
        }
    };

    // ========================================
    // WGS84 ellipsoid parameters (static members)
    // ========================================

    namespace WGS84
    {
        // Ellipsoid radii (meters)
        constexpr double RX = 6378137.0;                    // Equatorial radius (semi-major axis)
        constexpr double RY = 6378137.0;                    // Equatorial radius (semi-major axis)
        constexpr double RZ = 6356752.3142451793;           // Polar radius (semi-minor axis)

        // Flattening
        constexpr double FLATTENING = 1.0 / 298.257223563;

        // Eccentricity squared
        constexpr double ECCENTRICITY_SQUARED = 2.0 * FLATTENING - FLATTENING * FLATTENING;
    }

} // namespace cckit::geo
