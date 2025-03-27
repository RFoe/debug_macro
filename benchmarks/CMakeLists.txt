cmake_minimum_required (VERSION 3.14)
project (${PROJECT_NAME}_benchmarks VERSION 1.0.0 LANGUAGES CXX)

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

include (FetchContent)
set (FETCHCONTENT_QUIET ON)
set (FETCHCONTENT_UPDATES_DISCONNECTED ON)
set (BUILD_SHARED_LIBS OFF)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)
find_package (Git REQUIRED)

# 确保 Google Benchmark 可用
set (BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set (BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare (
    benchmark GIT_REPOSITORY "https://github.com/google/benchmark.git"
    GIT_TAG 090faecb454fbd6e6e17a75ef8146acb037118d4 PATCH_COMMAND ""
)
FetchContent_MakeAvailable (benchmark)
FetchContent_GetProperties (benchmark SOURCE_DIR BENCHMARK_INCLUDE_DIR)

# 启用测试
enable_testing ()

# 添加性能测试可执行文件
add_executable (${PROJECT_NAME}_benchmarks performance.cpp)

# 使用 benchmark 的 include 目录
target_include_directories (
    ${PROJECT_NAME}_benchmarks PRIVATE ${BENCHMARK_INCLUDE_DIR}
)

# 链接 Google Benchmark 库和项目库
target_link_libraries (
    ${PROJECT_NAME}_benchmarks PRIVATE ${PROJECT_NAME} # 项目库
                                       benchmark::benchmark
)

# 编译器特定的警告选项
if (MSVC)
    # MSVC 特定的编译选项
    target_compile_options (${PROJECT_NAME}_benchmarks PRIVATE /W4)
else ()
    # 其他编译器的警告选项
    target_compile_options (
        ${PROJECT_NAME}_benchmarks PRIVATE -Wall -Wextra -pedantic
    )
endif ()

# 添加性能测试到 CTest
add_test (NAME ${PROJECT_NAME}_benchmarks COMMAND ${PROJECT_NAME}_benchmarks)
