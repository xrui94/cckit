# cckit String Module
if(CCKIT_BUILD_SHARED)
    add_library(cckit-string SHARED
        src/String.cpp
    )
    target_compile_definitions(cckit-string PRIVATE CCKIT_BUILDING_DLL)
else()
    add_library(cckit-string STATIC
        src/String.cpp
    )
endif()

add_library(cckit::string ALIAS cckit-string)

# 包含目录（指向模块自己的 include）
target_include_directories(cckit-string
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 编译选项
if(MSVC)
    target_compile_options(cckit-string PRIVATE /W4)
else()
    target_compile_options(cckit-string PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 安装库
install(TARGETS cckit-string
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
