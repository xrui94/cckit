# CCKIT UUID Tests CMakeLists.txt

cmake_minimum_required(VERSION 3.15)
project(cckit_uuid_tests VERSION 1.0.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# 链接 cckit-uuid 库
if(TARGET cckit-uuid)
    message(STATUS "Found cckit-uuid target")
else()
    message(FATAL_ERROR "cckit-uuid target not found. Please build the main library first.")
endif()

# 测试可执行文件
add_executable(test_uuid_basic
    test_uuid_basic.cpp
)

target_link_libraries(test_uuid_basic
    PRIVATE
    cckit-uuid
)

add_executable(test_uuid_performance
    test_uuid_performance.cpp
)

target_link_libraries(test_uuid_performance
    PRIVATE
    cckit-uuid
)

# 启用测试
enable_testing()

# 添加测试
add_test(NAME uuid_basic COMMAND test_uuid_basic)
add_test(NAME uuid_performance COMMAND test_uuid_performance)

# 设置测试超时（秒）
set_tests_properties(uuid_basic PROPERTIES TIMEOUT 30)
set_tests_properties(uuid_performance PROPERTIES TIMEOUT 60)

# 输出测试信息
message(STATUS "CCKIT UUID Tests:")
message(STATUS "  - test_uuid_basic")
message(STATUS "  - test_uuid_performance")
