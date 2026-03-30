// Cartographic.hpp - Geodetic coordinates C++ interface
//
// Point in geodetic coordinate system (longitude, latitude, height)
// Named consistently with Cesium, with Geodetic alias for academic naming
//
// Design principles:
// 1. Main class name Cartographic (consistent with Cesium)
// 2. Type alias Geodetic (compatible with academic naming)
// 3. Support conversion to/from Cartesian3

#pragma once

// Enable M_PI on Windows
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

// Include C++ headers FIRST (before C API header)
#include <cmath>
#include <string>
#include <sstream>

// C API
#include "cckit/geo/capi/geo.h"

// C++ types
#include "Cartesian3.hpp"

// Fallback if M_PI is still not defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace cckit::geo
{

    // ========================================
    // Cartographic - Geodetic coordinates
    // ========================================

    /**
     * @brief Geodetic coordinates (longitude, latitude, height)
     *
     * Coordinate definition:
     * - longitude: Longitude in radians, range [-PI, PI]
     * - latitude: Latitude in radians, range [-PI/2, PI/2]
     * - height: Height in meters, height above ellipsoid
     *
     * Naming notes:
     * - Cartographic: Consistent with Cesium, easy for users to understand
     * - Geodetic: Academic standard naming, provided as alias
     */
    class Cartographic
    {
    public:
        double longitude;  // Longitude in radians
        double latitude;   // Latitude in radians
        double height;     // Height in meters

        // ========================================
        // Constructors
        // ========================================

        Cartographic() : longitude(0.0), latitude(0.0), height(0.0) {}

        Cartographic(double _longitude, double _latitude, double _height = 0.0)
            : longitude(_longitude), latitude(_latitude), height(_height) {}

        // Construct from C struct
        explicit Cartographic(const cckit_cartographic_t& c)
            : longitude(c.longitude), latitude(c.latitude), height(c.height) {}

        // ========================================
        // Static factory methods
        // ========================================

        /**
         * @brief Create from radians
         */
        static Cartographic fromRadians(double longitude, double latitude, double height = 0.0) {
            return Cartographic(longitude, latitude, height);
        }

        /**
         * @brief Create from degrees
         */
        static Cartographic fromDegrees(double longitude, double latitude, double height = 0.0) {
            return Cartographic(
                longitude * M_PI / 180.0,
                latitude * M_PI / 180.0,
                height
            );
        }

        /**
         * @brief Convert from Cartesian3
         */
        static Cartographic fromCartesian(const Cartesian3& cartesian);

        // ========================================
        // Member methods
        // ========================================

        /**
         * @brief Convert to Cartesian3
         */
        Cartesian3 toCartesian() const;

        /**
         * @brief Get longitude in degrees
         */
        double longitudeDegrees() const {
            return longitude * 180.0 / M_PI;
        }

        /**
         * @brief Get latitude in degrees
         */
        double latitudeDegrees() const {
            return latitude * 180.0 / M_PI;
        }

        // ========================================
        // Operator overloading
        // ========================================

        bool operator==(const Cartographic& other) const {
            return longitude == other.longitude &&
                   latitude == other.latitude &&
                   height == other.height;
        }

        bool operator!=(const Cartographic& other) const {
            return !(*this == other);
        }

        // ========================================
        // Conversion
        // ========================================

        /**
         * @brief Convert to C struct
         */
        cckit_cartographic_t toC() const {
            cckit_cartographic_t c;
            c.longitude = longitude;
            c.latitude = latitude;
            c.height = height;
            return c;
        }

        /**
         * @brief Convert from C struct
         */
        static Cartographic fromC(const cckit_cartographic_t& c) {
            return Cartographic(c.longitude, c.latitude, c.height);
        }

        /**
         * @brief Convert to string
         */
        std::string toString() const {
            std::ostringstream ss;
            ss << "(lon: " << longitudeDegrees() << " deg, "
               << "lat: " << latitudeDegrees() << " deg, "
               << "height: " << height << " m)";
            return ss.str();
        }
    };

    // ========================================
    // Type alias - Geodetic (academic naming)
    // ========================================

    /**
     * @brief Geodetic - Geodetic coordinates alias
     *
     * Uses academic standard naming, consistent with OGC, PROJ, GDAL, etc.
     * This is an alias for Cartographic, both are equivalent.
     */
    using Geodetic = Cartographic;

} // namespace cckit::geo
