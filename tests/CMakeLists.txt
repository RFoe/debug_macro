# 获取 GoogleTest
include (FetchContent)
FetchContent_Declare (
    googletest GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0 # 使用最新稳定版本
)

# 确保 Google Test 可用
set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable (googletest)

# 启用测试
enable_testing ()

# 添加测试可执行文件：标准测试
add_executable (debug_macro_tests basic.cpp # 可以添加更多测试文件
)

# 链接 Google Test 库
target_link_libraries (
    debug_macro_tests PRIVATE ${PROJECT_NAME} # 项目库
                              gtest_main gmock
)

# 使用 Google Test 发现测试
include (GoogleTest)
gtest_discover_tests (debug_macro_tests)

# 添加禁用宏的测试可执行文件
add_executable (debug_macro_tests_macro_disabled advanced.cpp)

target_link_libraries (
    debug_macro_tests_macro_disabled PRIVATE ${PROJECT_NAME} gtest_main gmock
)

gtest_discover_tests (debug_macro_tests_macro_disabled)

# 添加测试
add_test (NAME debug_macro_tests COMMAND debug_macro_tests)

add_test (NAME debug_macro_tests_macro_disabled
          COMMAND debug_macro_tests_macro_disabled
)

# 全局测试配置
if (MSVC)
    # MSVC 特定的测试编译选项
    target_compile_options (debug_macro_tests PRIVATE /W4)
    target_compile_options (debug_macro_tests_macro_disabled PRIVATE /W4)
else ()
    # 其他编译器的警告选项
    target_compile_options (debug_macro_tests PRIVATE -Wall -Wextra -pedantic)
    target_compile_options (
        debug_macro_tests_macro_disabled PRIVATE -Wall -Wextra -pedantic
    )
endif ()
