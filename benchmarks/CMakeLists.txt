# Modern CMake practices
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

# 显式设置Release模式
set (CMAKE_BUILD_TYPE Release)

# Fetch and configure Google Benchmark
include (FetchContent)
set (FETCHCONTENT_QUIET OFF)
set (FETCHCONTENT_UPDATES_DISCONNECTED ON)
set (BUILD_SHARED_LIBS OFF)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)
find_package (Git REQUIRED)

set (BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set (BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare (
    benchmark GIT_REPOSITORY "https://github.com/google/benchmark.git"
    GIT_TAG main PATCH_COMMAND ""
)
FetchContent_MakeAvailable (benchmark)
FetchContent_GetProperties (benchmark SOURCE_DIR BENCHMARK_INCLUDE_DIR)

message (STATUS "Benchmark source dir: ${benchmark_SOURCE_DIR}")
message (STATUS "Benchmark binary dir: ${benchmark_BINARY_DIR}")

# Add benchmark executable
add_executable (debug_macro_benchmarks performance.cpp)

# Add include directories
target_include_directories (
    debug_macro_benchmarks PRIVATE ${BENCHMARK_INCLUDE_DIR}
)

# Link against the project library and Google Benchmark
target_link_libraries (
    debug_macro_benchmarks PRIVATE debug_macro benchmark::benchmark_main
)

# Add benchmarks as CTest tests
add_test (NAME debug_macro_performance_benchmark COMMAND debug_macro_benchmarks
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Set properties for the benchmark test
set_tests_properties (
    debug_macro_performance_benchmark
    PROPERTIES LABELS "benchmark" TIMEOUT 300 # 5-minute timeout for benchmarks
)

# Optional: Add a custom target to run benchmarks
add_custom_target (
    run_benchmarks COMMAND ${CMAKE_CTEST_COMMAND} -L "benchmark"
    COMMENT "Running performance benchmarks"
)
