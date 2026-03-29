# CMakeLists.txt for cckit::random tests
cmake_minimum_required(VERSION 3.15)

# 查找测试框架
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    # 如果没有找到 GTest，使用 FetchContent 下载
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    # 对于 Windows：防止覆盖父项目的编译器/链接器设置
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# 创建测试可执行文件
add_executable(random_tests
    test_main.cpp
    test_random_generator.cpp
    test_distributions.cpp
    test_weighted.cpp
    test_compat.cpp
)

# 链接库
target_link_libraries(random_tests
    PRIVATE
        cckit::random
        GTest::gtest
        GTest::gtest_main
)

# 设置包含目录
target_include_directories(random_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# 设置 C++ 标准
target_compile_features(random_tests PUBLIC cxx_std_17)

# 注册测试
include(GoogleTest)
gtest_discover_tests(random_tests)
