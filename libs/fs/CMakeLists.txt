# cckit Filesystem Module
# 支持多种后端实现：std::filesystem 或 ghc::filesystem

# 设置文件系统后端选项
option(CCKIT_USE_GHC_FS "Use ghc::filesystem backend (for old compilers)" OFF)

# 确定使用哪个后端
# 默认使用 std::filesystem，除非显式指定 CCKIT_USE_GHC_FS=ON
if(CCKIT_USE_GHC_FS)
    set(CCKIT_FS_BACKEND "ghc")
    message(STATUS "cckit-fs backend: ghc::filesystem (CCKIT_USE_GHC_FS=ON)")
else()
    set(CCKIT_FS_BACKEND "std")
    message(STATUS "cckit-fs backend: std::filesystem (default)")
endif()

if(CCKIT_BUILD_SHARED)
    add_library(cckit-fs SHARED
        src/fs.cpp
    )
    target_compile_definitions(cckit-fs PRIVATE CCKIT_BUILDING_DLL)
else()
    add_library(cckit-fs STATIC
        src/fs.cpp
    )
endif()

add_library(cckit::fs ALIAS cckit-fs)

# 包含目录（指向模块自己的 include）
target_include_directories(cckit-fs
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 根据后端选择不同的依赖和宏定义
if(CCKIT_FS_BACKEND STREQUAL "ghc")
    # 使用 ghc::filesystem 后端
    find_package(ghc_filesystem CONFIG REQUIRED)
    set(FS_BACKEND_DEFINE CCKIT_USE_GHC_FS)
    # 链接依赖
    target_link_libraries(cckit-fs
        PUBLIC ghcFilesystem::ghc_filesystem
    )
else()
    # 使用 std::filesystem 后端
    set(FS_BACKEND_DEFINE CCKIT_USE_STD_FS)
endif()

# 定义后端宏
target_compile_definitions(cckit-fs PUBLIC ${FS_BACKEND_DEFINE})

# 编译选项
if(MSVC)
    target_compile_options(cckit-fs PRIVATE /W4)
else()
    target_compile_options(cckit-fs PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 安装库
install(TARGETS cckit-fs
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
