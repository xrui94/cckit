# cckit::runtime Module

# 源文件
set(RUNTIME_SOURCES
    src/Runtime.cpp
)

# 头文件
set(RUNTIME_HEADERS
    include/cckit/export.h
    include/cckit/version.h
    include/cckit/runtime/Runtime.hpp
    include/cckit/runtime/capi/runtime.h
)

# 创建动态库或静态库
if(CCKIT_BUILD_SHARED)
    add_library(cckit-runtime SHARED
        ${RUNTIME_SOURCES}
        ${RUNTIME_HEADERS}
    )
    target_compile_definitions(cckit-runtime PRIVATE CCKIT_BUILDING_DLL)
else()
    add_library(cckit-runtime STATIC
        ${RUNTIME_SOURCES}
        ${RUNTIME_HEADERS}
    )
endif()

# 设置别名
add_library(cckit::runtime ALIAS cckit-runtime)

# 包含目录（指向模块自己的 include）
target_include_directories(cckit-runtime
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Windows 特定链接库
if(WIN32)
    target_link_libraries(cckit-runtime PRIVATE
        userenv
        shell32
        psapi
    )
endif()

# 编译选项
if(MSVC)
    target_compile_options(cckit-runtime PRIVATE /W4)
else()
    target_compile_options(cckit-runtime PRIVATE -Wall -Wextra -Wpedantic)
endif()

# # 安装规则
# include(GNUInstallDirs)

# 安装库
install(TARGETS cckit-runtime
    EXPORT cckit-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# install(DIRECTORY include/
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )

# install(EXPORT cckit-runtime-targets
#     FILE cckit-runtime-targets.cmake
#     NAMESPACE cckit::
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cckit-runtime
# )

# # 创建配置文件
# include(CMakePackageConfigHelpers)

# write_basic_package_version_file(
#     "${CMAKE_CURRENT_BINARY_DIR}/cckit-runtime-config-version.cmake"
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY SameMajorVersion
# )

# configure_package_config_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cckit-runtime-config.cmake.in"
#     "${CMAKE_CURRENT_BINARY_DIR}/cckit-runtime-config.cmake"
#     INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cckit-runtime
# )

# install(FILES
#     "${CMAKE_CURRENT_BINARY_DIR}/cckit-runtime-config.cmake"
#     "${CMAKE_CURRENT_BINARY_DIR}/cckit-runtime-config-version.cmake"
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cckit-runtime
# )

# 测试
if(CCKIT_BUILD_TESTS)
    add_subdirectory(test)
endif()

# 示例
if(CCKIT_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
