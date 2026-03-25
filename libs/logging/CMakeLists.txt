# cckit Logging Module
find_package(spdlog CONFIG REQUIRED)

if(CCKIT_BUILD_SHARED)
    add_library(cckit-logging SHARED
        src/log.cpp
    )
    target_compile_definitions(cckit-logging PRIVATE CCKIT_BUILDING_DLL)
else()
    add_library(cckit-logging STATIC
        src/log.cpp
    )
endif()

add_library(cckit::logging ALIAS cckit-logging)

# 包含目录（指向模块自己的 include）
target_include_directories(cckit-logging
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 链接依赖
target_link_libraries(cckit-logging
    PUBLIC
        spdlog::spdlog_header_only
)

# 编译选项
if(MSVC)
    target_compile_options(cckit-logging PRIVATE /W4)
else()
    target_compile_options(cckit-logging PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 安装库
install(TARGETS cckit-logging
    EXPORT cckit-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 测试
if(CCKIT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# 示例
if(CCKIT_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
