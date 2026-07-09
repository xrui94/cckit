# CCKIT UUID Tests CMakeLists.txt

cmake_minimum_required(VERSION 3.15)
project(cckit_uuid_tests VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

if(TARGET cckit-uuid)
    message(STATUS "Found cckit-uuid target")
else()
    message(FATAL_ERROR "cckit-uuid target not found. Please build the main library first.")
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

add_executable(test_uuid_basic
    test_uuid_basic.cpp
)

target_link_libraries(test_uuid_basic
    PRIVATE
    cckit-uuid
    GTest::gtest
    GTest::gtest_main
)

add_executable(test_uuid_performance
    test_uuid_performance.cpp
)

target_link_libraries(test_uuid_performance
    PRIVATE
    cckit-uuid
    GTest::gtest
    GTest::gtest_main
)

add_test(NAME uuid_basic COMMAND test_uuid_basic)
add_test(NAME uuid_performance COMMAND test_uuid_performance)

set_tests_properties(uuid_basic PROPERTIES TIMEOUT 30)
set_tests_properties(uuid_performance PROPERTIES TIMEOUT 60)

message(STATUS "CCKIT UUID Tests:")
message(STATUS "  - test_uuid_basic")
message(STATUS "  - test_uuid_performance")
