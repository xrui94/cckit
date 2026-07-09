# CCKIT Filesystem Tests CMakeLists.txt

cmake_minimum_required(VERSION 3.15)
project(cckit_fs_tests VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

if(TARGET cckit-fs)
    message(STATUS "Found cckit-fs target")
else()
    message(FATAL_ERROR "cckit-fs target not found. Please build the main library first.")
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

add_executable(test_fs_basic
    test_fs_basic.cpp
)

target_link_libraries(test_fs_basic
    PRIVATE
    cckit-fs
    GTest::gtest
    GTest::gtest_main
)

add_executable(test_fs_performance
    test_fs_performance.cpp
)

target_link_libraries(test_fs_performance
    PRIVATE
    cckit-fs
    GTest::gtest
    GTest::gtest_main
)

add_test(NAME fs_basic COMMAND test_fs_basic)
add_test(NAME fs_performance COMMAND test_fs_performance)

set_tests_properties(fs_basic PROPERTIES TIMEOUT 60)
set_tests_properties(fs_performance PROPERTIES TIMEOUT 120)

message(STATUS "CCKIT Filesystem Tests:")
message(STATUS "  - test_fs_basic")
message(STATUS "  - test_fs_performance")
