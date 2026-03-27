# CCKIT Filesystem Tests CMakeLists.txt

cmake_minimum_required(VERSION 3.15)
project(cckit_fs_tests VERSION 1.0.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# 链接 cckit-fs 库
if(TARGET cckit-fs)
    message(STATUS "Found cckit-fs target")
else()
    message(FATAL_ERROR "cckit-fs target not found. Please build the main library first.")
endif()

# 测试可执行文件
add_executable(test_fs_basic
    test_fs_basic.cpp
)

target_link_libraries(test_fs_basic
    PRIVATE
    cckit-fs
)

add_executable(test_fs_performance
    test_fs_performance.cpp
)

target_link_libraries(test_fs_performance
    PRIVATE
    cckit-fs
)

# 添加测试
add_test(NAME fs_basic COMMAND test_fs_basic)
add_test(NAME fs_performance COMMAND test_fs_performance)

# 设置测试超时（秒）
set_tests_properties(fs_basic PROPERTIES TIMEOUT 60)
set_tests_properties(fs_performance PROPERTIES TIMEOUT 120)

# 输出测试信息
message(STATUS "CCKIT Filesystem Tests:")
message(STATUS "  - test_fs_basic")
message(STATUS "  - test_fs_performance")
