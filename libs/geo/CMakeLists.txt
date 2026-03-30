# geo sub-library - Geographic/GIS utilities
#
# Provides:
# - Cartesian3: Earth-Centered Earth-Fixed Cartesian coordinates
# - Cartographic: Geodetic coordinates (longitude, latitude, height)
# - Ellipsoid: Ellipsoid model (WGS84, etc.)
#
# Design principles:
# - Consistent with Cesium naming
# - Support conversion between geodetic and Cartesian coordinates

cmake_minimum_required(VERSION 3.14)

project(cckit-geo VERSION 0.1.0 LANGUAGES CXX)

# ========================================
# Options
# ========================================

option(CCKIT_GEO_BUILD_TESTS "Build geo sub-library tests" OFF)

# ========================================
# Library target
# ========================================

add_library(cckit-geo STATIC
    src/Geo.cpp
)

add_library(cckit::cckit-geo ALIAS cckit-geo)

# ========================================
# Compile options
# ========================================

target_compile_features(cckit-geo PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(cckit-geo PRIVATE
        /W4
        /WX-  # Warnings not as errors
    )
else()
    target_compile_options(cckit-geo PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# ========================================
# Include directories
# ========================================

target_include_directories(cckit-geo
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# ========================================
# Link libraries
# ========================================

# geo sub-library has no external dependencies

# ========================================
# Tests
# ========================================

if(CCKIT_GEO_BUILD_TESTS OR CCKIT_BUILD_TESTS)
    enable_testing()

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
        test/test_main.cpp
        test/test_cartesian3.cpp
        test/test_cartographic.cpp
    )

    target_link_libraries(geo-tests PRIVATE
        cckit-geo
        GTest::gtest
        GTest::gtest_main
    )

    # Add tests to ctest
    add_test(NAME Cartesian3Tests COMMAND geo-tests --gtest_filter=Cartesian3Test.*)
    add_test(NAME CartographicTests COMMAND geo-tests --gtest_filter=CartographicTest.*)
    add_test(NAME GeodeticTests COMMAND geo-tests --gtest_filter=GeodeticTest.*)
endif()

# 安装
# ========================================

# 注意：使用主项目的 cckit-targets export
# 不需要单独的 install(EXPORT) 和 config 文件

include(GNUInstallDirs)

install(TARGETS cckit-geo
    EXPORT cckit-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
