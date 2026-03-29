# cckit Random Module

if(CCKIT_BUILD_SHARED)
    add_library(cckit-random SHARED
        src/Random.cpp
    )
    target_compile_definitions(cckit-random PRIVATE CCKIT_BUILDING_DLL)
else()
    add_library(cckit-random STATIC
        src/Random.cpp
    )
endif()

add_library(cckit::random ALIAS cckit-random)

# 包含目录（指向模块自己的 include）
target_include_directories(cckit-random
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 编译选项
if(MSVC)
    target_compile_options(cckit-random PRIVATE /W4)
else()
    target_compile_options(cckit-random PRIVATE -Wall -Wextra -Wpedantic)
endif()

# C++17 标准（用于 std::random 和 thread_local）
target_compile_features(cckit-random PUBLIC cxx_std_17)

# 安装库
install(TARGETS cckit-random
    EXPORT cckit-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 测试
if(CCKIT_BUILD_TESTS)
    add_subdirectory(test)
endif()

# 示例
if(CCKIT_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
