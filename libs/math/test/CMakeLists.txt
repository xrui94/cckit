# CCKIT Math Tests CMakeLists.txt

cmake_minimum_required(VERSION 3.15)
project(cckit_math_tests VERSION 1.0.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# 链接 cckit-math 库
if(TARGET cckit-math)
    message(STATUS "Found cckit-math target")
else()
    message(FATAL_ERROR "cckit-math target not found. Please build the main library first.")
endif()

# 测试可执行文件
add_executable(test_math_basic
    test_math_basic.cpp
)

target_link_libraries(test_math_basic
    PRIVATE
    cckit-math
)

add_executable(test_math_performance
    test_math_performance.cpp
)

target_link_libraries(test_math_performance
    PRIVATE
    cckit-math
)

# 启用测试
enable_testing()

# 添加测试
add_test(NAME math_basic COMMAND test_math_basic)
add_test(NAME math_performance COMMAND test_math_performance)

# 设置测试超时（秒）
set_tests_properties(math_basic PROPERTIES TIMEOUT 30)
set_tests_properties(math_performance PROPERTIES TIMEOUT 60)

# 输出测试信息
message(STATUS "CCKIT Math Tests:")
message(STATUS "  - test_math_basic")
message(STATUS "  - test_math_performance")
