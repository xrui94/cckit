# cckit 顶层 CMakeLists.txt
cmake_minimum_required(VERSION 3.15...3.29)

project(cckit
    VERSION 1.0.0
    LANGUAGES C CXX
    DESCRIPTION "C++ Common Library - Modular library"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ========================================
# 模块化选项
# ========================================
option(CCKIT_BUILD_SHARED "Build as shared library (Not supported yet!)" OFF)
option(CCKIT_BUILD_TESTS "Build tests" OFF)
option(CCKIT_BUILD_EXAMPLES "Build examples" OFF)

option(CCKIT_BUILD_ALL "Build all modules" OFF)

option(CCKIT_BUILD_STRING "Build string module" ${CCKIT_BUILD_ALL})
option(CCKIT_BUILD_FS "Build filesystem module" ${CCKIT_BUILD_ALL})
option(CCKIT_BUILD_MATH "Build math module (requires GLM)" ${CCKIT_BUILD_ALL})
option(CCKIT_BUILD_LOGGING "Build logging module (requires spdlog)" ${CCKIT_BUILD_ALL})
option(CCKIT_BUILD_UUID "Build UUID module (requires Boost.UUID)" ${CCKIT_BUILD_ALL})

# ========================================
# 添加子模块
# ========================================
if(CCKIT_BUILD_STRING)
    add_subdirectory(libs/string)
endif()

if(CCKIT_BUILD_FS)
    add_subdirectory(libs/fs)
endif()

if(CCKIT_BUILD_MATH)
    add_subdirectory(libs/math)
endif()

if(CCKIT_BUILD_LOGGING)
    add_subdirectory(libs/logging)
endif()

if(CCKIT_BUILD_UUID)
    add_subdirectory(libs/uuid)
endif()

# ========================================
# 创建统一的目标（可选）
# ========================================
# 创建一个 INTERFACE 目标，方便用户链接所有模块

# 检查是否有模块被启用
set(CCKIT_HAS_MODULES FALSE)
if(CCKIT_BUILD_STRING)
    set(CCKIT_HAS_MODULES TRUE)
endif()
if(CCKIT_BUILD_FS)
    set(CCKIT_HAS_MODULES TRUE)
endif()
if(CCKIT_BUILD_MATH)
    set(CCKIT_HAS_MODULES TRUE)
endif()
if(CCKIT_BUILD_LOGGING)
    set(CCKIT_HAS_MODULES TRUE)
endif()
if(CCKIT_BUILD_UUID)
    set(CCKIT_HAS_MODULES TRUE)
endif()

# 如果没有启用任何模块，给出错误提示
if(NOT CCKIT_HAS_MODULES)
    message(FATAL_ERROR
        "CCKit requires at least one module to be enabled.\n"
        "Enable modules using one of the following options:\n"
        "  -DCCKIT_BUILD_ALL=ON (build all modules)\n"
        "  -DCCKIT_BUILD_STRING=ON\n"
        "  -DCCKIT_BUILD_FS=ON (default: std::filesystem, use -DCCKIT_USE_GHC_FS=ON for ghc::filesystem)\n"
        "  -DCCKIT_BUILD_MATH=ON\n"
        "  -DCCKIT_BUILD_LOGGING=ON\n"
        "  -DCCKIT_BUILD_UUID=ON"
    )
endif()

add_library(cckit INTERFACE)

# 链接所有启用的模块
if(CCKIT_BUILD_STRING)
    target_link_libraries(cckit INTERFACE cckit-string)
endif()

if(CCKIT_BUILD_FS)
    target_link_libraries(cckit INTERFACE cckit-fs)
endif()

if(CCKIT_BUILD_MATH)
    target_link_libraries(cckit INTERFACE cckit-math)
endif()

if(CCKIT_BUILD_LOGGING)
    target_link_libraries(cckit INTERFACE cckit-logging)
endif()

if(CCKIT_BUILD_UUID)
    target_link_libraries(cckit INTERFACE cckit-uuid)
endif()

add_library(cckit::cckit ALIAS cckit)

# ========================================
# 安装
# ========================================
include(GNUInstallDirs)

# 安装所有头文件（合并各个模块的头文件）
if(CCKIT_BUILD_STRING)
    install(DIRECTORY libs/string/include/cckit
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

if(CCKIT_BUILD_FS)
    install(DIRECTORY libs/fs/include/cckit
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

if(CCKIT_BUILD_GHC_FS)
    install(DIRECTORY libs/ghc-fs/include/cckit
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

if(CCKIT_BUILD_MATH)
    install(DIRECTORY libs/math/include/cckit
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

if(CCKIT_BUILD_LOGGING)
    install(DIRECTORY libs/logging/include/cckit
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

if(CCKIT_BUILD_UUID)
    install(DIRECTORY libs/uuid/include/cckit
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

# 安装 CMake 配置文件
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cckitConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cckitConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cckit
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/cckitConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/cckitConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cckitConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cckit
)

# 安装导出的 targets
install(EXPORT cckit-targets
    FILE cckitTargets.cmake
    NAMESPACE cckit::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cckit
)
