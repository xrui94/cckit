// test_cartographic.cpp - Cartographic tests
#include <gtest/gtest.h>
#include <cckit/geo/Geo.hpp>

using namespace cckit::geo;

// ========================================
// Cartographic Construction Tests
// ========================================

TEST(CartographicTest, DefaultConstructor)
{
    Cartographic c;
    EXPECT_DOUBLE_EQ(c.longitude, 0.0);
    EXPECT_DOUBLE_EQ(c.latitude, 0.0);
    EXPECT_DOUBLE_EQ(c.height, 0.0);
}

TEST(CartographicTest, ParameterizedConstructor)
{
    Cartographic c(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(c.longitude, 1.0);
    EXPECT_DOUBLE_EQ(c.latitude, 2.0);
    EXPECT_DOUBLE_EQ(c.height, 3.0);
}

TEST(CartographicTest, FromRadians)
{
    Cartographic c = Cartographic::fromRadians(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(c.longitude, 1.0);
    EXPECT_DOUBLE_EQ(c.latitude, 2.0);
    EXPECT_DOUBLE_EQ(c.height, 3.0);
}

TEST(CartographicTest, FromDegrees)
{
    Cartographic c = Cartographic::fromDegrees(90.0, 45.0, 100.0);
    EXPECT_NEAR(c.longitude, M_PI / 2.0, 1e-15);
    EXPECT_NEAR(c.latitude, M_PI / 4.0, 1e-15);
    EXPECT_DOUBLE_EQ(c.height, 100.0);
}

TEST(CartographicTest, LongitudeDegrees)
{
    Cartographic c = Cartographic::fromRadians(M_PI / 2.0, 0.0, 0.0);
    EXPECT_NEAR(c.longitudeDegrees(), 90.0, 1e-10);
}

TEST(CartographicTest, LatitudeDegrees)
{
    Cartographic c = Cartographic::fromRadians(0.0, M_PI / 4.0, 0.0);
    EXPECT_NEAR(c.latitudeDegrees(), 45.0, 1e-10);
}

// ========================================
// Cartographic Conversion Tests
// ========================================

TEST(CartographicTest, ToCartesian_Beijing)
{
    // Beijing: 116.4E, 39.9N, 0m
    Cartographic c = Cartographic::fromDegrees(116.4, 39.9, 0.0);
    Cartesian3 pos = c.toCartesian();
    
    // Expected values (calculated from WGS84 ellipsoid)
    // These are approximate values, use larger tolerance
    EXPECT_NEAR(pos.x / 1e6, -2.179, 0.01);
    EXPECT_NEAR(pos.y / 1e6, 4.389, 0.01);
    EXPECT_NEAR(pos.z / 1e6, 4.074, 0.01);
}

TEST(CartographicTest, FromCartesian_RoundTrip)
{
    // Test round-trip conversion
    Cartographic original = Cartographic::fromDegrees(116.4, 39.9, 100.0);
    Cartesian3 cartesian = original.toCartesian();
    Cartographic result = Cartographic::fromCartesian(cartesian);
    
    EXPECT_NEAR(result.longitudeDegrees(), 116.4, 0.0001);
    EXPECT_NEAR(result.latitudeDegrees(), 39.9, 0.0001);
    EXPECT_NEAR(result.height, 100.0, 0.1);
}

TEST(CartographicTest, ToCartesian_EquatorPrimeMeridian)
{
    // 0, 0, 0 should be on equator at prime meridian
    Cartographic c = Cartographic::fromDegrees(0.0, 0.0, 0.0);
    Cartesian3 pos = c.toCartesian();
    
    // Should be on equator at prime meridian (X = WGS84_RX, Y = 0, Z = 0)
    EXPECT_NEAR(pos.x, 6378137.0, 1.0);
    EXPECT_NEAR(pos.y, 0.0, 1.0);
    EXPECT_NEAR(pos.z, 0.0, 1.0);
}

TEST(CartographicTest, ToCartesian_NorthPole)
{
    // North pole: 90N
    Cartographic c = Cartographic::fromDegrees(0.0, 90.0, 0.0);
    Cartesian3 pos = c.toCartesian();
    
    // Should be at Z = WGS84_RZ
    EXPECT_NEAR(pos.x, 0.0, 1.0);
    EXPECT_NEAR(pos.y, 0.0, 1.0);
    EXPECT_NEAR(pos.z, 6356752.3142451793, 1.0);
}

TEST(CartographicTest, ToCartesian_SouthPole)
{
    // South pole: 90S
    Cartographic c = Cartographic::fromDegrees(0.0, -90.0, 0.0);
    Cartesian3 pos = c.toCartesian();
    
    // Should be at Z = -WGS84_RZ
    EXPECT_NEAR(pos.x, 0.0, 1.0);
    EXPECT_NEAR(pos.y, 0.0, 1.0);
    EXPECT_NEAR(pos.z, -6356752.3142451793, 1.0);
}

// ========================================
// Cartographic Operator Tests
// ========================================

TEST(CartographicTest, OperatorEqual)
{
    Cartographic a = Cartographic::fromDegrees(10.0, 20.0, 30.0);
    Cartographic b = Cartographic::fromDegrees(10.0, 20.0, 30.0);
    EXPECT_TRUE(a == b);
}

TEST(CartographicTest, OperatorNotEqual)
{
    Cartographic a = Cartographic::fromDegrees(10.0, 20.0, 30.0);
    Cartographic b = Cartographic::fromDegrees(10.0, 20.0, 31.0);
    EXPECT_TRUE(a != b);
}

// ========================================
// Geodetic Alias Tests
// ========================================

TEST(GeodeticTest, AliasWorks)
{
    // Geodetic is an alias for Cartographic
    Geodetic g = Geodetic::fromDegrees(116.4, 39.9, 100.0);
    EXPECT_NEAR(g.longitudeDegrees(), 116.4, 1e-10);
    EXPECT_NEAR(g.latitudeDegrees(), 39.9, 1e-10);
    EXPECT_DOUBLE_EQ(g.height, 100.0);
}

TEST(GeodeticTest, ConversionToCartesian)
{
    Geodetic g = Geodetic::fromDegrees(0.0, 0.0, 0.0);
    Cartesian3 pos = g.toCartesian();
    
    EXPECT_NEAR(pos.x, 6378137.0, 1.0);
    EXPECT_NEAR(pos.y, 0.0, 1.0);
    EXPECT_NEAR(pos.z, 0.0, 1.0);
}

// ========================================
// Cartographic Edge Cases
// ========================================

TEST(CartographicTest, HeightVariation)
{
    // Same location, different heights
    Cartographic seaLevel = Cartographic::fromDegrees(0.0, 0.0, 0.0);
    Cartographic highAlt = Cartographic::fromDegrees(0.0, 0.0, 10000.0);
    
    Cartesian3 posSea = seaLevel.toCartesian();
    Cartesian3 posHigh = highAlt.toCartesian();
    
    // Higher altitude should be further from origin
    EXPECT_GT(Cartesian3::magnitude(posHigh), Cartesian3::magnitude(posSea));
}

TEST(CartographicTest, LongitudeWrapAround)
{
    // 180E and 180W should be same location
    Cartographic east = Cartographic::fromDegrees(180.0, 0.0, 0.0);
    Cartographic west = Cartographic::fromDegrees(-180.0, 0.0, 0.0);
    
    Cartesian3 posEast = east.toCartesian();
    Cartesian3 posWest = west.toCartesian();
    
    EXPECT_NEAR(posEast.x, posWest.x, 1e-6);
    EXPECT_NEAR(posEast.y, posWest.y, 1e-6);
    EXPECT_NEAR(posEast.z, posWest.z, 1e-6);
}
