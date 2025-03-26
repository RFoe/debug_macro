# 基本用法示例
add_executable (basic_usage_example basic_usage.cpp)
target_link_libraries (basic_usage_example PRIVATE ${PROJECT_NAME})

# 高级用法示例
add_executable (advanced_usage_example advanced_usage.cpp)
target_link_libraries (advanced_usage_example PRIVATE ${PROJECT_NAME})

# 编译器特定警告和优化选项
if (MSVC)
    target_compile_options (basic_usage_example PRIVATE /W4)
    target_compile_options (advanced_usage_example PRIVATE /W4)
else ()
    target_compile_options (basic_usage_example PRIVATE -Wall -Wextra -pedantic)
    target_compile_options (
        advanced_usage_example PRIVATE -Wall -Wextra -pedantic
    )
endif ()

# 可选：添加示例到安装目录
install (TARGETS basic_usage_example advanced_usage_example
         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/examples
)
