#include <array>
#include <benchmark/benchmark.h>
#include <debug_macro.hpp>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

// 性能测试工具宏
#define BENCHMARK_DEBUG_MACRO_TYPE(Type, Name) \
  static void BM_DebugMacro_##Name(benchmark::State &state) { \
    Type value{}; \
    for (auto _: state) { debug_macro(value); } \
  } \
  BENCHMARK(BM_DebugMacro_##Name)

// 性能测试工具宏：容器类型
#define BENCHMARK_DEBUG_MACRO_CONTAINER(ContainerType, ValueType, Name) \
  static void BM_DebugMacro_##Name(benchmark::State &state) { \
    ContainerType<ValueType> container(100); \
    for (auto _: state) { debug_macro(container); } \
  } \
  BENCHMARK(BM_DebugMacro_##Name)
// 自定义复杂结构体
struct ComplexStruct {
  int              id;
  std::string      name;
  double           value;
  std::vector<int> data;
  ComplexStruct()
    : id(42)
    , name("BenchmarkStruct")
    , value(3.14159)
    , data(100, 42) {}
};
// 原生类型性能测试
BENCHMARK_DEBUG_MACRO_TYPE(int, Integer);
BENCHMARK_DEBUG_MACRO_TYPE(double, Double);
BENCHMARK_DEBUG_MACRO_TYPE(std::string, String);
BENCHMARK_DEBUG_MACRO_TYPE(ComplexStruct, CustomStruct);

// 标准容器性能测试
BENCHMARK_DEBUG_MACRO_CONTAINER(std::vector, int, Vector_Int);
BENCHMARK_DEBUG_MACRO_CONTAINER(std::vector, std::string, Vector_String);
BENCHMARK_DEBUG_MACRO_CONTAINER(std::list, int, List_Int);
BENCHMARK_DEBUG_MACRO_CONTAINER(std::set, int, Set_Int);
BENCHMARK_DEBUG_MACRO_CONTAINER(std::map, std::pair<int, std::string>, Map);
// 高级类型性能测试
static void
BM_DebugMacro_Tuple(benchmark::State &state) {
  std::tuple<int, std::string, double> tuple{42, "benchmark", 3.14};
  for (auto _: state) { debug_macro(tuple); }
}
BENCHMARK(BM_DebugMacro_Tuple);
static void
BM_DebugMacro_Optional(benchmark::State &state) {
  std::optional<int> opt = 42;
  for (auto _: state) { debug_macro(opt); }
}
BENCHMARK(BM_DebugMacro_Optional);
static void
BM_DebugMacro_SmartPointer(benchmark::State &state) {
  auto unique_ptr = std::make_unique<int>(42);
  for (auto _: state) { debug_macro(unique_ptr); }
}
BENCHMARK(BM_DebugMacro_SmartPointer);
// 多变量性能测试
static void
BM_DebugMacro_MultipleVariables(benchmark::State &state) {
  int         x   = 42;
  std::string str = "benchmark";
  double      pi  = 3.14159;
  for (auto _: state) { debug_macro(x, str, pi); }
}
BENCHMARK(BM_DebugMacro_MultipleVariables);
// 嵌套容器性能测试
static void
BM_DebugMacro_NestedContainers(benchmark::State &state) {
  std::vector<std::vector<int>> nested_vec(10, std::vector<int>(10, 42));
  for (auto _: state) { debug_macro(nested_vec); }
}
BENCHMARK(BM_DebugMacro_NestedContainers);
// 特殊类型性能测试
enum TestEnum { RED, GREEN, BLUE };
static void
BM_DebugMacro_Enum(benchmark::State &state) {
  TestEnum color = GREEN;
  for (auto _: state) { debug_macro(color); }
}
BENCHMARK(BM_DebugMacro_Enum);
// 大量数据性能测试
static void
BM_DebugMacro_LargeVector(benchmark::State &state) {
  std::vector<int> large_vec(10000, 42);
  for (auto _: state) { debug_macro(large_vec); }
}
BENCHMARK(BM_DebugMacro_LargeVector);

// 运行基准测试
BENCHMARK_MAIN();
