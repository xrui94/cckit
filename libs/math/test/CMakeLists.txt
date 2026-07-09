# CCKIT Math Tests CMakeLists.txt

cmake_minimum_required(VERSION 3.15)
project(cckit_math_tests VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

if(TARGET cckit-math)
    message(STATUS "Found cckit-math target")
else()
    message(FATAL_ERROR "cckit-math target not found. Please build the main library first.")
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

add_executable(test_math_basic
    test_math_basic.cpp
)

target_link_libraries(test_math_basic
    PRIVATE
    cckit-math
    GTest::gtest
    GTest::gtest_main
)

add_executable(test_math_performance
    test_math_performance.cpp
)

target_link_libraries(test_math_performance
    PRIVATE
    cckit-math
    GTest::gtest
    GTest::gtest_main
)

add_test(NAME math_basic COMMAND test_math_basic)
add_test(NAME math_performance COMMAND test_math_performance)

set_tests_properties(math_basic PROPERTIES TIMEOUT 30)
set_tests_properties(math_performance PROPERTIES TIMEOUT 60)

message(STATUS "CCKIT Math Tests:")
message(STATUS "  - test_math_basic")
message(STATUS "  - test_math_performance")
