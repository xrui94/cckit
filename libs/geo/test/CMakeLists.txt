# CCKIT Geo Tests CMakeLists.txt

cmake_minimum_required(VERSION 3.14)
project(cckit_geo_tests VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

if(TARGET cckit-geo)
    message(STATUS "Found cckit-geo target")
else()
    message(FATAL_ERROR "cckit-geo target not found. Please build the main library first.")
endif()

find_package(GTest QUIET)

if(NOT GTest_FOUND)
    message(STATUS "GTest not found, downloading...")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(geo-tests
    test_main.cpp
    test_cartesian3.cpp
    test_cartographic.cpp
)

target_link_libraries(geo-tests PRIVATE
    cckit-geo
    GTest::gtest
    GTest::gtest_main
)

add_test(NAME Cartesian3Tests COMMAND geo-tests --gtest_filter=Cartesian3Test.*)
add_test(NAME CartographicTests COMMAND geo-tests --gtest_filter=CartographicTest.*)
add_test(NAME GeodeticTests COMMAND geo-tests --gtest_filter=GeodeticTest.*)

set_tests_properties(Cartesian3Tests PROPERTIES TIMEOUT 30)
set_tests_properties(CartographicTests PROPERTIES TIMEOUT 30)
set_tests_properties(GeodeticTests PROPERTIES TIMEOUT 30)

message(STATUS "CCKIT Geo Tests:")
message(STATUS "  - Cartesian3Tests")
message(STATUS "  - CartographicTests")
message(STATUS "  - GeodeticTests")
