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

# 添加测试可执行文件
add_executable (basic_test basic.cpp # 可以添加更多测试文件
)

# 链接 Google Test 库
target_link_libraries (
    basic_test
    PRIVATE ${PROJECT_NAME} # 您的项目库
            gtest_main # Google Test 主入口
            gmock # Google Mock (可选)
)

# 使用 Google Test 发现测试
include (GoogleTest)
gtest_discover_tests (basic_test)

# 可以添加更多测试文件
add_executable (advanced_test advanced.cpp)

target_link_libraries (advanced_test PRIVATE ${PROJECT_NAME} gtest_main gmock)

gtest_discover_tests (advanced_test)

# 全局测试配置
if (MSVC)
    # MSVC 特定的测试编译选项
    target_compile_options (basic_test PRIVATE /W4)
    target_compile_options (advanced_test PRIVATE /W4)
else ()
    # 其他编译器的警告选项
    target_compile_options (basic_test PRIVATE -Wall -Wextra -pedantic)
    target_compile_options (advanced_test PRIVATE -Wall -Wextra -pedantic)
endif ()
