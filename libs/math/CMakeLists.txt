# cckit Math Module
find_package(glm CONFIG REQUIRED)

if(CCKIT_BUILD_SHARED)
    add_library(cckit-math SHARED
        src/Vec2.cpp
        src/Vec3.cpp
        src/Vec4.cpp
        src/Mat3.cpp
        src/Mat4.cpp
        src/Quat.cpp
    )
    target_compile_definitions(cckit-math PRIVATE CCKIT_BUILDING_DLL)
else()
    add_library(cckit-math STATIC
        src/Vec2.cpp
        src/Vec3.cpp
        src/Vec4.cpp
        src/Mat3.cpp
        src/Mat4.cpp
        src/Quat.cpp
    )
endif()

add_library(cckit::math ALIAS cckit-math)

# 包含目录（指向模块自己的 include）
target_include_directories(cckit-math
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 链接依赖
target_link_libraries(cckit-math
    PUBLIC
        glm::glm
)

# 编译选项
if(MSVC)
    target_compile_options(cckit-math PRIVATE /W4)
else()
    target_compile_options(cckit-math PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 安装库
install(TARGETS cckit-math
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
