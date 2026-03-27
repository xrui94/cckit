# CCKIT String Tests CMakeLists.txt

cmake_minimum_required(VERSION 3.15)
project(cckit_string_tests VERSION 1.0.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# 链接 cckit-string 库
if(TARGET cckit-string)
    message(STATUS "Found cckit-string target")
else()
    message(FATAL_ERROR "cckit-string target not found. Please build the main library first.")
endif()

# 测试可执行文件
add_executable(test_string_basic
    test_string_basic.cpp
)

target_link_libraries(test_string_basic
    PRIVATE
    cckit-string
)

add_executable(test_string_performance
    test_string_performance.cpp
)

target_link_libraries(test_string_performance
    PRIVATE
    cckit-string
)

# 启用测试
enable_testing()

# 添加测试
add_test(NAME string_basic COMMAND test_string_basic)
add_test(NAME string_performance COMMAND test_string_performance)

# 设置测试超时（秒）
set_tests_properties(string_basic PROPERTIES TIMEOUT 30)
set_tests_properties(string_performance PROPERTIES TIMEOUT 60)

# 输出测试信息
message(STATUS "CCKIT String Tests:")
message(STATUS "  - test_string_basic")
message(STATUS "  - test_string_performance")
