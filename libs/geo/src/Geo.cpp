// Geo.cpp - Geographic/GIS utilities implementation
//
// Implements C API and C++ API

// Include C++ headers FIRST
#include <cmath>
#include <algorithm>
#include <utility>

#include "cckit/geo/Geo.hpp"

namespace cckit::geo
{

    // ========================================
    // Internal constants
    // ========================================

    namespace
    {
        const double EPSILON1 = 0.1;
        const double EPSILON12 = 1e-12;

        // WGS84 ellipsoid parameters
        const double WGS84_RX = 6378137.0;
        const double WGS84_RY = 6378137.0;
        const double WGS84_RZ = 6356752.3142451793;

        double sign(double value)
        {
            if (value > 0) return 1.0;
            if (value < 0) return -1.0;
            return 0.0;
        }
    }

    // ========================================
    // C API implementation
    // ========================================

    extern "C"
    {

    // Cartesian3 C API

    cckit_cartesian3_t cckit_cartesian3_create(double x, double y, double z)
    {
        cckit_cartesian3_t result;
        result.x = x;
        result.y = y;
        result.z = z;
        return result;
    }

    cckit_cartesian3_t cckit_cartesian3_from_degrees(double longitude, double latitude, double height)
    {
        return cckit_cartesian3_from_radians(
            longitude * RADIANS_PER_DEGREE,
            latitude * RADIANS_PER_DEGREE,
            height
        );
    }

    cckit_cartesian3_t cckit_cartesian3_from_radians(double longitude, double latitude, double height)
    {
        // WGS84 ellipsoid parameters
        double radiiSquaredX = WGS84_RX * WGS84_RX;
        double radiiSquaredY = WGS84_RY * WGS84_RY;
        double radiiSquaredZ = WGS84_RZ * WGS84_RZ;

        double cosLatitude = std::cos(latitude);
        double cosLongitude = std::cos(longitude);
        double sinLatitude = std::sin(latitude);
        double sinLongitude = std::sin(longitude);

        // Compute N (unit normal vector)
        double nX = cosLatitude * cosLongitude;
        double nY = cosLatitude * sinLongitude;
        double nZ = sinLatitude;

        // Normalize
        double nMag = std::sqrt(nX * nX + nY * nY + nZ * nZ);
        nX /= nMag;
        nY /= nMag;
        nZ /= nMag;

        // Compute K = radiiSquared * N
        double kX = radiiSquaredX * nX;
        double kY = radiiSquaredY * nY;
        double kZ = radiiSquaredZ * nZ;

        // gamma = sqrt(N . K)
        double gamma = std::sqrt(nX * kX + nY * kY + nZ * kZ);

        // K = K / gamma
        kX /= gamma;
        kY /= gamma;
        kZ /= gamma;

        // result = K + N * height
        cckit_cartesian3_t result;
        result.x = kX + nX * height;
        result.y = kY + nY * height;
        result.z = kZ + nZ * height;
        return result;
    }

    void cckit_cartesian3_to_degrees(cckit_cartesian3_t cartesian, double* longitude, double* latitude, double* height)
    {
        cckit_cartesian3_to_radians(cartesian, longitude, latitude, height);
        *longitude *= DEGREES_PER_RADIAN;
        *latitude *= DEGREES_PER_RADIAN;
    }

    void cckit_cartesian3_to_radians(cckit_cartesian3_t cartesian, double* longitude, double* latitude, double* height)
    {
        // Use WGS84 parameters
        double oneOverRadiiX = 1.0 / WGS84_RX;
        double oneOverRadiiY = 1.0 / WGS84_RY;
        double oneOverRadiiZ = 1.0 / WGS84_RZ;
        double oneOverRadiiSquaredX = oneOverRadiiX * oneOverRadiiX;
        double oneOverRadiiSquaredY = oneOverRadiiY * oneOverRadiiY;
        double oneOverRadiiSquaredZ = oneOverRadiiZ * oneOverRadiiZ;

        // Scale to geodetic surface
        cckit_cartesian3_t oneOverRadii = {oneOverRadiiX, oneOverRadiiY, oneOverRadiiZ};
        cckit_cartesian3_t oneOverRadiiSquared = {oneOverRadiiSquaredX, oneOverRadiiSquaredY, oneOverRadiiSquaredZ};

        cckit_cartesian3_t p = cckit_cartesian3_scale_to_geodetic_surface(
            cartesian, oneOverRadii, oneOverRadiiSquared, EPSILON1);

        // n = normalize(p * oneOverRadiiSquared)
        double nX = p.x * oneOverRadiiSquaredX;
        double nY = p.y * oneOverRadiiSquaredY;
        double nZ = p.z * oneOverRadiiSquaredZ;
        double nMag = std::sqrt(nX * nX + nY * nY + nZ * nZ);
        nX /= nMag;
        nY /= nMag;
        nZ /= nMag;

        // h = cartesian - p
        double hX = cartesian.x - p.x;
        double hY = cartesian.y - p.y;
        double hZ = cartesian.z - p.z;
        double hMag = std::sqrt(hX * hX + hY * hY + hZ * hZ);

        // height = sign(h . cartesian) * |h|
        double hDotC = hX * cartesian.x + hY * cartesian.y + hZ * cartesian.z;

        *longitude = std::atan2(nY, nX);
        *latitude = std::asin(nZ);
        *height = sign(hDotC) * hMag;
    }

    cckit_cartesian3_t cckit_cartesian3_add(cckit_cartesian3_t left, cckit_cartesian3_t right)
    {
        cckit_cartesian3_t result;
        result.x = left.x + right.x;
        result.y = left.y + right.y;
        result.z = left.z + right.z;
        return result;
    }

    cckit_cartesian3_t cckit_cartesian3_subtract(cckit_cartesian3_t left, cckit_cartesian3_t right)
    {
        cckit_cartesian3_t result;
        result.x = left.x - right.x;
        result.y = left.y - right.y;
        result.z = left.z - right.z;
        return result;
    }

    double cckit_cartesian3_dot(cckit_cartesian3_t left, cckit_cartesian3_t right)
    {
        return left.x * right.x + left.y * right.y + left.z * right.z;
    }

    cckit_cartesian3_t cckit_cartesian3_cross(cckit_cartesian3_t left, cckit_cartesian3_t right)
    {
        cckit_cartesian3_t result;
        result.x = left.y * right.z - left.z * right.y;
        result.y = left.z * right.x - left.x * right.z;
        result.z = left.x * right.y - left.y * right.x;
        return result;
    }

    double cckit_cartesian3_magnitude(cckit_cartesian3_t cartesian)
    {
        return std::sqrt(cckit_cartesian3_magnitude_squared(cartesian));
    }

    double cckit_cartesian3_magnitude_squared(cckit_cartesian3_t cartesian)
    {
        return cartesian.x * cartesian.x + cartesian.y * cartesian.y + cartesian.z * cartesian.z;
    }

    cckit_cartesian3_t cckit_cartesian3_normalize(cckit_cartesian3_t cartesian)
    {
        double mag = cckit_cartesian3_magnitude(cartesian);
        cckit_cartesian3_t result;
        result.x = cartesian.x / mag;
        result.y = cartesian.y / mag;
        result.z = cartesian.z / mag;
        return result;
    }

    cckit_cartesian3_t cckit_cartesian3_multiply_by_scalar(cckit_cartesian3_t cartesian, double scalar)
    {
        cckit_cartesian3_t result;
        result.x = cartesian.x * scalar;
        result.y = cartesian.y * scalar;
        result.z = cartesian.z * scalar;
        return result;
    }

    cckit_cartesian3_t cckit_cartesian3_divide_by_scalar(cckit_cartesian3_t cartesian, double scalar)
    {
        cckit_cartesian3_t result;
        result.x = cartesian.x / scalar;
        result.y = cartesian.y / scalar;
        result.z = cartesian.z / scalar;
        return result;
    }

    cckit_cartesian3_t cckit_cartesian3_multiply_components(cckit_cartesian3_t left, cckit_cartesian3_t right)
    {
        cckit_cartesian3_t result;
        result.x = left.x * right.x;
        result.y = left.y * right.y;
        result.z = left.z * right.z;
        return result;
    }

    // Internal function: scale to geodetic surface
    cckit_cartesian3_t cckit_cartesian3_scale_to_geodetic_surface(
        cckit_cartesian3_t cartesian,
        cckit_cartesian3_t oneOverRadii,
        cckit_cartesian3_t oneOverRadiiSquared,
        double centerToleranceSquared)
    {
        double positionX = cartesian.x;
        double positionY = cartesian.y;
        double positionZ = cartesian.z;

        double oneOverRadiiX = oneOverRadii.x;
        double oneOverRadiiY = oneOverRadii.y;
        double oneOverRadiiZ = oneOverRadii.z;

        double x2 = positionX * positionX * oneOverRadiiX * oneOverRadiiX;
        double y2 = positionY * positionY * oneOverRadiiY * oneOverRadiiY;
        double z2 = positionZ * positionZ * oneOverRadiiZ * oneOverRadiiZ;

        double squaredNorm = x2 + y2 + z2;
        double ratio = std::sqrt(1.0 / squaredNorm);

        cckit_cartesian3_t intersection = cckit_cartesian3_multiply_by_scalar(cartesian, ratio);

        if (squaredNorm < centerToleranceSquared)
        {
            return intersection;
        }

        double oneOverRadiiSquaredX = oneOverRadiiSquared.x;
        double oneOverRadiiSquaredY = oneOverRadiiSquared.y;
        double oneOverRadiiSquaredZ = oneOverRadiiSquared.z;

        double gradientX = intersection.x * oneOverRadiiSquaredX * 2.0;
        double gradientY = intersection.y * oneOverRadiiSquaredY * 2.0;
        double gradientZ = intersection.z * oneOverRadiiSquaredZ * 2.0;
        double gradientMag = std::sqrt(gradientX * gradientX + gradientY * gradientY + gradientZ * gradientZ);

        double cartesianMag = cckit_cartesian3_magnitude(cartesian);
        double lambda = (1.0 - ratio) * cartesianMag / (0.5 * gradientMag);
        double correction = 0.0;

        double func, denominator;
        double xMultiplier, yMultiplier, zMultiplier;
        double xMultiplier2, yMultiplier2, zMultiplier2;
        double xMultiplier3, yMultiplier3, zMultiplier3;

        do
        {
            lambda -= correction;

            xMultiplier = 1.0 / (1.0 + lambda * oneOverRadiiSquaredX);
            yMultiplier = 1.0 / (1.0 + lambda * oneOverRadiiSquaredY);
            zMultiplier = 1.0 / (1.0 + lambda * oneOverRadiiSquaredZ);

            xMultiplier2 = xMultiplier * xMultiplier;
            yMultiplier2 = yMultiplier * yMultiplier;
            zMultiplier2 = zMultiplier * zMultiplier;

            xMultiplier3 = xMultiplier2 * xMultiplier;
            yMultiplier3 = yMultiplier2 * yMultiplier;
            zMultiplier3 = zMultiplier2 * zMultiplier;

            func = x2 * xMultiplier2 + y2 * yMultiplier2 + z2 * zMultiplier2 - 1.0;

            denominator = x2 * xMultiplier3 * oneOverRadiiSquaredX +
                          y2 * yMultiplier3 * oneOverRadiiSquaredY +
                          z2 * zMultiplier3 * oneOverRadiiSquaredZ;

            double derivative = -2.0 * denominator;
            correction = func / derivative;
        } while (std::abs(func) > EPSILON12);

        cckit_cartesian3_t result;
        result.x = positionX * xMultiplier;
        result.y = positionY * yMultiplier;
        result.z = positionZ * zMultiplier;
        return result;
    }

    // Cartographic C API

    cckit_cartographic_t cckit_cartographic_from_radians(double longitude, double latitude, double height)
    {
        cckit_cartographic_t result;
        result.longitude = longitude;
        result.latitude = latitude;
        result.height = height;
        return result;
    }

    cckit_cartographic_t cckit_cartographic_from_degrees(double longitude, double latitude, double height)
    {
        cckit_cartographic_t result;
        result.longitude = longitude * RADIANS_PER_DEGREE;
        result.latitude = latitude * RADIANS_PER_DEGREE;
        result.height = height;
        return result;
    }

    cckit_cartographic_t cckit_cartographic_from_cartesian(cckit_cartesian3_t cartesian)
    {
        double longitude, latitude, height;
        cckit_cartesian3_to_radians(cartesian, &longitude, &latitude, &height);
        return cckit_cartographic_from_radians(longitude, latitude, height);
    }

    cckit_cartesian3_t cckit_cartographic_to_cartesian(cckit_cartographic_t cartographic)
    {
        return cckit_cartesian3_from_radians(
            cartographic.longitude,
            cartographic.latitude,
            cartographic.height
        );
    }

    // Ellipsoid C API

    cckit_ellipsoid_t cckit_ellipsoid_wgs84(void)
    {
        return cckit_ellipsoid_create(WGS84_RX, WGS84_RY, WGS84_RZ);
    }

    cckit_ellipsoid_t cckit_ellipsoid_create(double x_radius, double y_radius, double z_radius)
    {
        cckit_ellipsoid_t e;
        e.x_radius = x_radius;
        e.y_radius = y_radius;
        e.z_radius = z_radius;
        e.x_radius_squared = x_radius * x_radius;
        e.y_radius_squared = y_radius * y_radius;
        e.z_radius_squared = z_radius * z_radius;
        e.one_over_x_radius = 1.0 / x_radius;
        e.one_over_y_radius = 1.0 / y_radius;
        e.one_over_z_radius = 1.0 / z_radius;
        e.one_over_x_radius_squared = 1.0 / e.x_radius_squared;
        e.one_over_y_radius_squared = 1.0 / e.y_radius_squared;
        e.one_over_z_radius_squared = 1.0 / e.z_radius_squared;
        return e;
    }

    // Math constants C API

    double cckit_geo_radians_to_degrees(double radians)
    {
        return radians * DEGREES_PER_RADIAN;
    }

    double cckit_geo_degrees_to_radians(double degrees)
    {
        return degrees * RADIANS_PER_DEGREE;
    }

    } // extern "C"

    // ========================================
    // C++ API implementation - Cartesian3
    // ========================================

    Cartesian3 Cartesian3::fromDegree(double lon, double lat, double height)
    {
        return fromRadians(lon * RADIANS_PER_DEGREE, lat * RADIANS_PER_DEGREE, height);
    }

    Cartesian3 Cartesian3::fromRadians(double longitude, double latitude, double height)
    {
        cckit_cartesian3_t c = cckit_cartesian3_from_radians(longitude, latitude, height);
        return fromC(c);
    }

    Cartesian3 Cartesian3::add(Cartesian3 left, Cartesian3 right)
    {
        return fromC(cckit_cartesian3_add(left.toC(), right.toC()));
    }

    Cartesian3 Cartesian3::subtract(Cartesian3 left, Cartesian3 right)
    {
        return fromC(cckit_cartesian3_subtract(left.toC(), right.toC()));
    }

    double Cartesian3::dot(Cartesian3 left, Cartesian3 right)
    {
        return cckit_cartesian3_dot(left.toC(), right.toC());
    }

    Cartesian3 Cartesian3::cross(Cartesian3 left, Cartesian3 right)
    {
        return fromC(cckit_cartesian3_cross(left.toC(), right.toC()));
    }

    double Cartesian3::magnitudeSquared(Cartesian3 cartesian)
    {
        return cckit_cartesian3_magnitude_squared(cartesian.toC());
    }

    double Cartesian3::magnitude(Cartesian3 cartesian)
    {
        return cckit_cartesian3_magnitude(cartesian.toC());
    }

    Cartesian3 Cartesian3::normalize(Cartesian3 cartesian)
    {
        return fromC(cckit_cartesian3_normalize(cartesian.toC()));
    }

    Cartesian3 Cartesian3::multiplyByScalar(Cartesian3 cartesian, double scalar)
    {
        return fromC(cckit_cartesian3_multiply_by_scalar(cartesian.toC(), scalar));
    }

    Cartesian3 Cartesian3::divideByScalar(Cartesian3 cartesian, double scalar)
    {
        return fromC(cckit_cartesian3_divide_by_scalar(cartesian.toC(), scalar));
    }

    Cartesian3 Cartesian3::multiplyComponents(Cartesian3 left, Cartesian3 right)
    {
        return fromC(cckit_cartesian3_multiply_components(left.toC(), right.toC()));
    }

    Cartesian3 Cartesian3::scaleToGeodeticSurface(
        Cartesian3 cartesian,
        Cartesian3 oneOverRadii,
        Cartesian3 oneOverRadiiSquared,
        double centerToleranceSquared)
    {
        return fromC(cckit_cartesian3_scale_to_geodetic_surface(
            cartesian.toC(), oneOverRadii.toC(), oneOverRadiiSquared.toC(), centerToleranceSquared));
    }

    void Cartesian3::toRadians(double& longitude, double& latitude, double& height)
    {
        cckit_cartesian3_to_radians(toC(), &longitude, &latitude, &height);
    }

    void Cartesian3::toDegrees(double& longitude, double& latitude, double& height)
    {
        cckit_cartesian3_to_degrees(toC(), &longitude, &latitude, &height);
    }

    // Horizon culling related functions
    Cartesian3 Cartesian3::computeHorizonCullingPointFromVertices(
        Cartesian3 directionToPoint,
        double* vertices,
        int elementSize,
        int stride,
        Cartesian3 center)
    {
        auto scaledSpaceDirectionToPoint = computeScaledSpaceDirectionToPoint(directionToPoint);
        double resultMagnitude = 0.0;

        Cartesian3 positionScratch;

        for (int i = 0; i < elementSize; i += stride)
        {
            positionScratch.x = vertices[i] + center.x;
            positionScratch.y = vertices[i + 1] + center.y;
            positionScratch.z = vertices[i + 2] + center.z;

            double candidateMagnitude = computeMagnitude(positionScratch, scaledSpaceDirectionToPoint);
            resultMagnitude = std::fmax(resultMagnitude, candidateMagnitude);
        }

        return magnitudeToPoint(scaledSpaceDirectionToPoint, resultMagnitude);
    }

    Cartesian3 Cartesian3::magnitudeToPoint(Cartesian3 scaledSpaceDirectionToPoint, double resultMagnitude)
    {
        if (resultMagnitude <= 0.0 || std::isnan(resultMagnitude))
        {
            return Cartesian3();
        }
        return multiplyByScalar(scaledSpaceDirectionToPoint, resultMagnitude);
    }

    Cartesian3 Cartesian3::computeScaledSpaceDirectionToPoint(Cartesian3 directionToPoint)
    {
        Cartesian3 directionToPointScratch = transformPositionToScaledSpace(directionToPoint);
        return normalize(directionToPointScratch);
    }

    Cartesian3 Cartesian3::transformPositionToScaledSpace(Cartesian3 directionToPoint)
    {
        Cartesian3 oneOverRadii(1.0 / WGS84_RX, 1.0 / WGS84_RY, 1.0 / WGS84_RZ);
        return multiplyComponents(directionToPoint, oneOverRadii);
    }

    Cartesian3 Cartesian3::transformScaledSpaceToPosition(Cartesian3 directionToPoint)
    {
        Cartesian3 radii(WGS84_RX, WGS84_RY, WGS84_RZ);
        return multiplyComponents(directionToPoint, radii);
    }

    double Cartesian3::computeMagnitude(Cartesian3 position, Cartesian3 scaledSpaceDirectionToPoint)
    {
        auto scaledSpacePosition = transformPositionToScaledSpace(position);
        auto magSquared = magnitudeSquared(scaledSpacePosition);
        auto mag = std::sqrt(magSquared);
        auto direction = divideByScalar(scaledSpacePosition, mag);

        magSquared = std::fmax(1.0, magSquared);
        mag = std::fmax(1.0, mag);

        auto cosAlpha = dot(direction, scaledSpaceDirectionToPoint);
        auto sinAlpha = magnitude(cross(direction, scaledSpaceDirectionToPoint));
        auto cosBeta = 1.0 / mag;
        auto sinBeta = std::sqrt(magSquared - 1.0) * cosBeta;

        return 1.0 / (cosAlpha * cosBeta - sinAlpha * sinBeta);
    }

    // ========================================
    // C++ API implementation - Cartographic
    // ========================================

    Cartographic Cartographic::fromCartesian(const Cartesian3& cartesian)
    {
        cckit_cartographic_t c = cckit_cartographic_from_cartesian(cartesian.toC());
        return fromC(c);
    }

    Cartesian3 Cartographic::toCartesian() const
    {
        return Cartesian3::fromC(cckit_cartographic_to_cartesian(toC()));
    }

} // namespace cckit::geo
