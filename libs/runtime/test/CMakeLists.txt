# CMakeLists.txt for cckit::runtime tests
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
add_executable(runtime_tests
    test_main.cpp
    test_path.cpp
    test_process.cpp
    test_environment.cpp
    test_directory.cpp
    test_system.cpp
    test_time.cpp
)

# 链接库
target_link_libraries(runtime_tests
    PRIVATE
        cckit::runtime
        GTest::gtest
        GTest::gtest_main
)

# 设置包含目录
target_include_directories(runtime_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# 设置 C++ 标准
target_compile_features(runtime_tests PUBLIC cxx_std_17)

# 注册测试
include(GoogleTest)
gtest_discover_tests(runtime_tests)
