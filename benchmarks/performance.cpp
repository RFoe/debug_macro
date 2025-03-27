#include <benchmark/benchmark.h>
#include <debug_macro.hpp>
#include <random>
#include <sstream>
// 性能测试数据生成器（支持可变规模）
class PerformanceTestDataGenerator {
public:
  // 生成不同大小的数据结构
  template <typename T> static std::vector<T> generate_vector(size_t size) {
    std::vector<T>     vec(size);
    std::random_device rd;
    std::mt19937       gen(rd());

    if constexpr (std::is_integral_v<T>) {
      std::uniform_int_distribution<> dis(0, 1000);
      std::generate(vec.begin(), vec.end(), [&]() { return dis(gen); });
    } else if constexpr (std::is_floating_point_v<T>) {
      std::uniform_real_distribution<> dis(0.0, 1000.0);
      std::generate(vec.begin(), vec.end(), [&]() { return dis(gen); });
    }

    return vec;
  }
  // 生成复杂结构体
  struct ComplexStruct {
    int              id;
    std::string      name;
    double           value;
    std::vector<int> nested_data;
  };
  static ComplexStruct generate_complex_struct(size_t nested_size) {
    return {42, "Complex Struct", 3.14159, generate_vector<int>(nested_size)};
  }
};
// 时间复杂度测试：不同输入规模下的性能
static void
BM_TimeComplexity(benchmark::State &state) {
  // 获取当前测试的输入规模
  int size = state.range(0);

  // 生成不同大小的测试数据
  auto vec_int     = PerformanceTestDataGenerator::generate_vector<int>(size);
  auto vec_complex = PerformanceTestDataGenerator::generate_vector<
    PerformanceTestDataGenerator::ComplexStruct>(size);

  for (auto _: state) {
    // 测试不同类型和规模的输出性能
    debug_macro(vec_int);
    debug_macro(vec_complex);

    // 防止编译器优化
    benchmark::DoNotOptimize(vec_int);
    benchmark::DoNotOptimize(vec_complex);
  }

  // 报告处理的数据量
  state.SetItemsProcessed(state.iterations() * size);
}
// 测试不同输入规模：从8到8192，每次乘2
BENCHMARK(BM_TimeComplexity)
  ->RangeMultiplier(2)
  ->Range(8, 8192)
  ->Complexity(benchmark::oN); // 分析时间复杂度
// 空间复杂度分析
static void
BM_SpaceComplexity(benchmark::State &state) {
  int size = state.range(0);

  for (auto _: state) {
    // 生成不同大小的数据结构
    auto vec = PerformanceTestDataGenerator::generate_vector<int>(size);
    auto complex_struct =
      PerformanceTestDataGenerator::generate_complex_struct(size);

    // 测试输出
    debug_macro(vec);
    debug_macro(complex_struct);

    // 报告内存使用
    state.SetBytesProcessed(
      vec.size() * sizeof(int) +
      sizeof(PerformanceTestDataGenerator::ComplexStruct) +
      complex_struct.nested_data.size() * sizeof(int));
  }

  state.SetItemsProcessed(state.iterations() * size);
}
BENCHMARK(BM_SpaceComplexity)->RangeMultiplier(2)->Range(8, 8192);
// 格式化开销测试
static void
BM_FormattingOverhead(benchmark::State &state) {
  int iterations = state.range(0);

  for (auto _: state) {
    // 模拟不同复杂度的格式化操作
    for (int i = 0; i < iterations; ++i) {
      std::ostringstream oss;
      oss << "Formatting complex data: " << i << " " << 3.14159 << " "
          << "string representation";

      benchmark::DoNotOptimize(oss.str());
    }
  }

  state.SetItemsProcessed(state.iterations() * iterations);
}
BENCHMARK(BM_FormattingOverhead)->Arg(10)->Arg(100)->Arg(1000);
// 内存分配频率分析
static void
BM_MemoryAllocationFrequency(benchmark::State &state) {
  int alloc_count = state.range(0);

  for (auto _: state) {
    // 模拟频繁内存分配场景
    std::vector<std::unique_ptr<int>> dynamic_allocations;

    for (int i = 0; i < alloc_count; ++i) {
      dynamic_allocations.push_back(std::make_unique<int>(i));
    }

    benchmark::DoNotOptimize(dynamic_allocations);
  }

  state.SetItemsProcessed(state.iterations() * alloc_count);
}
BENCHMARK(BM_MemoryAllocationFrequency)->Arg(10)->Arg(100)->Arg(1000);
// 性能抖动检测
static void
BM_PerformanceVariability(benchmark::State &state) {
  // 生成随机数生成器
  std::random_device              rd;
  std::mt19937                    gen(rd());
  std::uniform_int_distribution<> dis(1, 1000);

  for (auto _: state) {
    // 模拟可能引起性能抖动的操作
    std::vector<int> random_data(100);
    std::generate(
      random_data.begin(), random_data.end(), [&]() { return dis(gen); });

    // 使用复杂的输出操作
    debug_macro(random_data);

    benchmark::DoNotOptimize(random_data);
  }

  // 计算统计信息
  state.SetItemsProcessed(state.iterations() * 100);
}
BENCHMARK(BM_PerformanceVariability)
  ->Repetitions(10)             // 多次重复测试
  ->ReportAggregatesOnly(true); // 只报告聚合统计
// 多参数类型组合性能测试
static void
BM_MultiTypeOutput(benchmark::State &state) {
  auto int_vec = PerformanceTestDataGenerator::generate_vector<int>(100);
  auto complex_struct =
    PerformanceTestDataGenerator::generate_complex_struct(50);
  std::string str = "Multi-type output test";

  for (auto _: state) {
    debug_macro(int_vec, complex_struct, str);

    benchmark::DoNotOptimize(int_vec);
    benchmark::DoNotOptimize(complex_struct);
    benchmark::DoNotOptimize(str);
  }

  state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_MultiTypeOutput);

// 性能测试主函数
BENCHMARK_MAIN();
