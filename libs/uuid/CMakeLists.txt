# cckit UUID Module
find_package(Boost COMPONENTS uuid REQUIRED)

if(CCKIT_BUILD_SHARED)
    add_library(cckit-uuid SHARED
        src/Uuid.cpp
    )
    target_compile_definitions(cckit-uuid PRIVATE CCKIT_BUILDING_DLL)
else()
    add_library(cckit-uuid STATIC
        src/Uuid.cpp
    )
endif()

add_library(cckit::uuid ALIAS cckit-uuid)

# 包含目录（指向模块自己的 include）
target_include_directories(cckit-uuid
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 链接依赖
target_link_libraries(cckit-uuid
    PUBLIC
        Boost::uuid
)

# 编译选项
if(MSVC)
    target_compile_options(cckit-uuid PRIVATE /W4)
else()
    target_compile_options(cckit-uuid PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 安装库
install(TARGETS cckit-uuid
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
