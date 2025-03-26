# 检查是否有性能测试框架
find_package(benchmark QUIET)

if(benchmark_FOUND)
    # 性能基准测试可执行文件
    add_executable(debug_macro_benchmarks performance.cpp)
   
    target_link_libraries(debug_macro_benchmarks
        PRIVATE
        ${PROJECT_NAME}
        benchmark::benchmark
        benchmark::benchmark_main
    )
    
    # 添加性能测试
    add_test(
        NAME PerformanceBenchmark
        COMMAND debug_macro_benchmarks
    )
    
    # 性能测试配置
    set_tests_properties(PerformanceBenchmark
        PROPERTIES
        TIMEOUT 60
    )
else()
    message(WARNING "Google Benchmark not found. Performance benchmarks will be disabled.")
endif()
