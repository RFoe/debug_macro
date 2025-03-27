cmake_minimum_required (VERSION 3.14)
project (tests VERSION 1.0.0 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

# 检测CPU核心数并设置并行编译
include (ProcessorCount)
ProcessorCount (CPU_CORES)

# 如果检测到CPU核心数，则设置并行编译
if (CPU_CORES GREATER 0)
    # 针对不同编译器的并行编译设置
    if (MSVC)
        # Visual Studio编译器
        add_compile_options ("/MP${CPU_CORES}")
    elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        # GCC/G++编译器
        add_compile_options ("-j${CPU_CORES}")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # Clang编译器
        add_compile_options ("-j${CPU_CORES}")
    endif ()

    # 设置并行构建选项
    set (CMAKE_BUILD_TYPE_PARALLEL ON)
    set (CMAKE_PARALLEL_COMPILE_JOBS ${CPU_CORES})
endif ()

# 获取 GoogleTest
include (FetchContent)
set (FETCHCONTENT_QUIET ON)
set (FETCHCONTENT_UPDATES_DISCONNECTED ON)
set (BUILD_SHARED_LIBS OFF)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)
find_package (Git REQUIRED)

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
add_executable (debug_macro_tests2 advanced.cpp)

target_link_libraries (
    debug_macro_tests2 PRIVATE ${PROJECT_NAME} gtest_main gmock
)

gtest_discover_tests (debug_macro_tests2)

# 添加测试
add_test (NAME debug_macro_tests COMMAND debug_macro_tests)

add_test (NAME debug_macro_tests2 COMMAND debug_macro_tests2)

# 全局测试配置
if (MSVC)
    # MSVC 特定的测试编译选项
    target_compile_options (debug_macro_tests PRIVATE /W4)
    target_compile_options (debug_macro_tests2 PRIVATE /W4)
else ()
    # 其他编译器的警告选项
    target_compile_options (debug_macro_tests PRIVATE -Wall -Wextra -pedantic)
    target_compile_options (debug_macro_tests2 PRIVATE -Wall -Wextra -pedantic)
endif ()
