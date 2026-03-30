// test_cartesian3.cpp - Cartesian3 tests
#include <gtest/gtest.h>
#include <cckit/geo/Geo.hpp>

using namespace cckit::geo;

// ========================================
// Cartesian3 Construction Tests
// ========================================

TEST(Cartesian3Test, DefaultConstructor)
{
    Cartesian3 v;
    EXPECT_DOUBLE_EQ(v.x, 0.0);
    EXPECT_DOUBLE_EQ(v.y, 0.0);
    EXPECT_DOUBLE_EQ(v.z, 0.0);
}

TEST(Cartesian3Test, ParameterizedConstructor)
{
    Cartesian3 v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v.x, 1.0);
    EXPECT_DOUBLE_EQ(v.y, 2.0);
    EXPECT_DOUBLE_EQ(v.z, 3.0);
}

TEST(Cartesian3Test, FromCStruct)
{
    cckit_cartesian3_t c = {1.0, 2.0, 3.0};
    Cartesian3 v(c);
    EXPECT_DOUBLE_EQ(v.x, 1.0);
    EXPECT_DOUBLE_EQ(v.y, 2.0);
    EXPECT_DOUBLE_EQ(v.z, 3.0);
}

// ========================================
// Cartesian3 Vector Operations Tests
// ========================================

TEST(Cartesian3Test, Add)
{
    Cartesian3 a(1.0, 2.0, 3.0);
    Cartesian3 b(4.0, 5.0, 6.0);
    Cartesian3 result = Cartesian3::add(a, b);
    EXPECT_DOUBLE_EQ(result.x, 5.0);
    EXPECT_DOUBLE_EQ(result.y, 7.0);
    EXPECT_DOUBLE_EQ(result.z, 9.0);
}

TEST(Cartesian3Test, Subtract)
{
    Cartesian3 a(4.0, 5.0, 6.0);
    Cartesian3 b(1.0, 2.0, 3.0);
    Cartesian3 result = Cartesian3::subtract(a, b);
    EXPECT_DOUBLE_EQ(result.x, 3.0);
    EXPECT_DOUBLE_EQ(result.y, 3.0);
    EXPECT_DOUBLE_EQ(result.z, 3.0);
}

TEST(Cartesian3Test, Dot)
{
    Cartesian3 a(1.0, 2.0, 3.0);
    Cartesian3 b(4.0, 5.0, 6.0);
    double result = Cartesian3::dot(a, b);
    EXPECT_DOUBLE_EQ(result, 32.0);  // 1*4 + 2*5 + 3*6 = 32
}

TEST(Cartesian3Test, Cross)
{
    Cartesian3 a(1.0, 0.0, 0.0);
    Cartesian3 b(0.0, 1.0, 0.0);
    Cartesian3 result = Cartesian3::cross(a, b);
    EXPECT_DOUBLE_EQ(result.x, 0.0);
    EXPECT_DOUBLE_EQ(result.y, 0.0);
    EXPECT_DOUBLE_EQ(result.z, 1.0);  // X x Y = Z
}

TEST(Cartesian3Test, Magnitude)
{
    Cartesian3 v(3.0, 4.0, 0.0);
    EXPECT_DOUBLE_EQ(Cartesian3::magnitude(v), 5.0);
}

TEST(Cartesian3Test, MagnitudeSquared)
{
    Cartesian3 v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(Cartesian3::magnitudeSquared(v), 14.0);  // 1 + 4 + 9 = 14
}

TEST(Cartesian3Test, Normalize)
{
    Cartesian3 v(3.0, 4.0, 0.0);
    Cartesian3 n = Cartesian3::normalize(v);
    EXPECT_DOUBLE_EQ(n.x, 0.6);
    EXPECT_DOUBLE_EQ(n.y, 0.8);
    EXPECT_DOUBLE_EQ(n.z, 0.0);
}

TEST(Cartesian3Test, MultiplyByScalar)
{
    Cartesian3 v(1.0, 2.0, 3.0);
    Cartesian3 result = Cartesian3::multiplyByScalar(v, 2.0);
    EXPECT_DOUBLE_EQ(result.x, 2.0);
    EXPECT_DOUBLE_EQ(result.y, 4.0);
    EXPECT_DOUBLE_EQ(result.z, 6.0);
}

TEST(Cartesian3Test, DivideByScalar)
{
    Cartesian3 v(2.0, 4.0, 6.0);
    Cartesian3 result = Cartesian3::divideByScalar(v, 2.0);
    EXPECT_DOUBLE_EQ(result.x, 1.0);
    EXPECT_DOUBLE_EQ(result.y, 2.0);
    EXPECT_DOUBLE_EQ(result.z, 3.0);
}

TEST(Cartesian3Test, MultiplyComponents)
{
    Cartesian3 a(1.0, 2.0, 3.0);
    Cartesian3 b(4.0, 5.0, 6.0);
    Cartesian3 result = Cartesian3::multiplyComponents(a, b);
    EXPECT_DOUBLE_EQ(result.x, 4.0);
    EXPECT_DOUBLE_EQ(result.y, 10.0);
    EXPECT_DOUBLE_EQ(result.z, 18.0);
}

// ========================================
// Cartesian3 Operator Tests
// ========================================

TEST(Cartesian3Test, OperatorAdd)
{
    Cartesian3 a(1.0, 2.0, 3.0);
    Cartesian3 b(4.0, 5.0, 6.0);
    Cartesian3 result = a + b;
    EXPECT_DOUBLE_EQ(result.x, 5.0);
    EXPECT_DOUBLE_EQ(result.y, 7.0);
    EXPECT_DOUBLE_EQ(result.z, 9.0);
}

TEST(Cartesian3Test, OperatorSubtract)
{
    Cartesian3 a(4.0, 5.0, 6.0);
    Cartesian3 b(1.0, 2.0, 3.0);
    Cartesian3 result = a - b;
    EXPECT_DOUBLE_EQ(result.x, 3.0);
    EXPECT_DOUBLE_EQ(result.y, 3.0);
    EXPECT_DOUBLE_EQ(result.z, 3.0);
}

TEST(Cartesian3Test, OperatorMultiplyScalar)
{
    Cartesian3 v(1.0, 2.0, 3.0);
    Cartesian3 result = v * 2.0;
    EXPECT_DOUBLE_EQ(result.x, 2.0);
    EXPECT_DOUBLE_EQ(result.y, 4.0);
    EXPECT_DOUBLE_EQ(result.z, 6.0);
}

TEST(Cartesian3Test, OperatorDivideScalar)
{
    Cartesian3 v(2.0, 4.0, 6.0);
    Cartesian3 result = v / 2.0;
    EXPECT_DOUBLE_EQ(result.x, 1.0);
    EXPECT_DOUBLE_EQ(result.y, 2.0);
    EXPECT_DOUBLE_EQ(result.z, 3.0);
}

TEST(Cartesian3Test, OperatorNegate)
{
    Cartesian3 v(1.0, 2.0, 3.0);
    Cartesian3 result = -v;
    EXPECT_DOUBLE_EQ(result.x, -1.0);
    EXPECT_DOUBLE_EQ(result.y, -2.0);
    EXPECT_DOUBLE_EQ(result.z, -3.0);
}

TEST(Cartesian3Test, OperatorEqual)
{
    Cartesian3 a(1.0, 2.0, 3.0);
    Cartesian3 b(1.0, 2.0, 3.0);
    EXPECT_TRUE(a == b);
}

TEST(Cartesian3Test, OperatorNotEqual)
{
    Cartesian3 a(1.0, 2.0, 3.0);
    Cartesian3 b(1.0, 2.0, 4.0);
    EXPECT_TRUE(a != b);
}

// ========================================
// Cartesian3 Geodetic Conversion Tests
// ========================================

TEST(Cartesian3Test, FromDegrees_Beijing)
{
    // Beijing: 116.4E, 39.9N
    Cartesian3 pos = Cartesian3::fromDegree(116.4, 39.9, 0.0);
    
    // Expected values (calculated from WGS84 ellipsoid)
    // These are approximate values, use larger tolerance
    EXPECT_NEAR(pos.x / 1e6, -2.179, 0.01);
    EXPECT_NEAR(pos.y / 1e6, 4.389, 0.01);
    EXPECT_NEAR(pos.z / 1e6, 4.074, 0.01);
}

TEST(Cartesian3Test, FromRadians_Beijing)
{
    // Beijing in radians
    double lon = 116.4 * M_PI / 180.0;
    double lat = 39.9 * M_PI / 180.0;
    Cartesian3 pos = Cartesian3::fromRadians(lon, lat, 0.0);
    
    // Should be same as fromDegrees
    Cartesian3 pos2 = Cartesian3::fromDegree(116.4, 39.9, 0.0);
    EXPECT_NEAR(pos.x, pos2.x, 1e-6);
    EXPECT_NEAR(pos.y, pos2.y, 1e-6);
    EXPECT_NEAR(pos.z, pos2.z, 1e-6);
}

TEST(Cartesian3Test, ToDegrees_RoundTrip)
{
    // Test round-trip conversion
    Cartesian3 pos = Cartesian3::fromDegree(116.4, 39.9, 100.0);
    
    double lon, lat, h;
    pos.toDegrees(lon, lat, h);
    
    EXPECT_NEAR(lon, 116.4, 0.0001);
    EXPECT_NEAR(lat, 39.9, 0.0001);
    EXPECT_NEAR(h, 100.0, 0.1);
}

TEST(Cartesian3Test, ToRadians_RoundTrip)
{
    // Test round-trip conversion
    Cartesian3 pos = Cartesian3::fromDegree(116.4, 39.9, 100.0);
    
    double lon, lat, h;
    pos.toRadians(lon, lat, h);
    
    EXPECT_NEAR(lon * 180.0 / M_PI, 116.4, 0.0001);
    EXPECT_NEAR(lat * 180.0 / M_PI, 39.9, 0.0001);
    EXPECT_NEAR(h, 100.0, 0.1);
}

// ========================================
// Cartesian3 Edge Cases
// ========================================

TEST(Cartesian3Test, ZeroVector)
{
    Cartesian3 zero;
    EXPECT_DOUBLE_EQ(Cartesian3::magnitude(zero), 0.0);
}

TEST(Cartesian3Test, UnitVectors)
{
    Cartesian3 unitX(1.0, 0.0, 0.0);
    Cartesian3 unitY(0.0, 1.0, 0.0);
    Cartesian3 unitZ(0.0, 0.0, 1.0);
    
    EXPECT_DOUBLE_EQ(Cartesian3::magnitude(unitX), 1.0);
    EXPECT_DOUBLE_EQ(Cartesian3::magnitude(unitY), 1.0);
    EXPECT_DOUBLE_EQ(Cartesian3::magnitude(unitZ), 1.0);
    
    // Orthogonality
    EXPECT_DOUBLE_EQ(Cartesian3::dot(unitX, unitY), 0.0);
    EXPECT_DOUBLE_EQ(Cartesian3::dot(unitY, unitZ), 0.0);
    EXPECT_DOUBLE_EQ(Cartesian3::dot(unitZ, unitX), 0.0);
}
