# CCKIT Logging Tests CMakeLists.txt

cmake_minimum_required(VERSION 3.15)
project(cckit_logging_tests VERSION 1.0.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 查找依赖
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${spdlog_INCLUDE_DIRS}
    ${fmt_INCLUDE_DIRS}
)

# 链接 cckit-logging 库
# 确保主库已经构建
if(TARGET cckit-logging)
    message(STATUS "Found cckit-logging target")
else()
    message(FATAL_ERROR "cckit-logging target not found. Please build the main library first.")
endif()

# 测试可执行文件
add_executable(test_callback_thread_safety
    test_callback_thread_safety.cpp
)

target_link_libraries(test_callback_thread_safety
    PRIVATE
        cckit-logging  # ✅ 链接主库
)

add_executable(test_deadlock_prevention
    test_deadlock_prevention.cpp
)

target_link_libraries(test_deadlock_prevention
    PRIVATE
        cckit-logging  # ✅ 链接主库
)

add_executable(test_use_after_free
    test_use_after_free.cpp
)

target_link_libraries(test_use_after_free
    PRIVATE
        cckit-logging  # ✅ 链接主库
)

# 启用测试
enable_testing()

# 添加测试
add_test(NAME callback_thread_safety COMMAND test_callback_thread_safety)
add_test(NAME deadlock_prevention COMMAND test_deadlock_prevention)
add_test(NAME use_after_free COMMAND test_use_after_free)

# 设置测试超时（秒）
set_tests_properties(callback_thread_safety PROPERTIES TIMEOUT 30)
set_tests_properties(deadlock_prevention PROPERTIES TIMEOUT 30)
set_tests_properties(use_after_free PROPERTIES TIMEOUT 30)

# 输出测试信息
message(STATUS "CCKIT Logging Tests:")
message(STATUS "  - test_callback_thread_safety")
message(STATUS "  - test_deadlock_prevention")
message(STATUS "  - test_use_after_free")
