#include <debug_macro.hpp>
//
#include <gtest/gtest.h>
#include <list>
//

// Capture stdout for testing
class StdoutCapture {
private:
  std::streambuf   *old_buf;
  std::stringstream ss;
public:
  StdoutCapture() { old_buf = std::cout.rdbuf(ss.rdbuf()); }
  ~StdoutCapture() { std::cout.rdbuf(old_buf); }
  std::string getOutput() { return ss.str(); }
};
// Custom type for testing
struct TestStruct {
  int    a = 8;
  double b = 2.32;
  // Custom formatter
  template <class Char>
  friend std::basic_ostream<Char> &operator<<(std::basic_ostream<Char> &os,
                                              TestStruct const         &obj) {
    return os << "TestStruct(a=" << obj.a << ", b=" << obj.b << ")";
  }
};
// Enum for testing
enum TestEnum : unsigned char {
  RED  = 8,
  BLUE = 122,
};
TEST(DebugMacroAdvancedTest, VectorOutput) {
  StdoutCapture    capture;
  std::vector<int> vec{1, 2, 3, 4, 5};
  debug_macro(vec);
  std::string output = capture.getOutput();

  EXPECT_TRUE(output.find("vec = [1, 2, 3, 4, 5]") != std::string::npos);
}
TEST(DebugMacroAdvancedTest, ListOutput) {
  StdoutCapture          capture;
  std::list<std::string> list{"hello", "world"};
  debug_macro(list);
  std::string output = capture.getOutput();

  EXPECT_TRUE(output.find("list = [\"hello\", \"world\"]") !=
              std::string::npos);
}
TEST(DebugMacroAdvancedTest, MapOutput) {
  StdoutCapture              capture;
  std::map<std::string, int> map{{"a", 1}, {"b", 2}};
  debug_macro(map);
  std::string output = capture.getOutput();

  EXPECT_TRUE(output.find("map = {\"a\": 1, \"b\": 2}") != std::string::npos);
}
TEST(DebugMacroAdvancedTest, QueueOutput) {
  StdoutCapture   capture;
  std::queue<int> queue;
  queue.push(1);
  queue.push(2);
  queue.push(3);
  debug_macro(queue);
  std::string output = capture.getOutput();

  EXPECT_TRUE(output.find("queue = [1, 2, 3]") != std::string::npos);
}
TEST(DebugMacroAdvancedTest, TupleOutput) {
  StdoutCapture                        capture;
  std::tuple<int, double, std::string> tuple{1, 2.2, "hello"};
  debug_macro(tuple);
  std::string output = capture.getOutput();

  EXPECT_TRUE(output.find("tuple = (1, 2.2, \"hello\")") != std::string::npos);
}
TEST(DebugMacroAdvancedTest, OptionalOutput) {
  StdoutCapture      capture;
  std::optional<int> opt = 42;
  debug_macro(opt);
  std::string output = capture.getOutput();

  EXPECT_TRUE(output.find("opt = 42") != std::string::npos);

  std::optional<int> empty_opt;
  StdoutCapture      capture2;
  debug_macro(empty_opt);
  std::string output2 = capture2.getOutput();

  EXPECT_TRUE(output2.find("empty_opt = std::nullopt") != std::string::npos);
}
TEST(DebugMacroAdvancedTest, SmartPointerOutput) {
  StdoutCapture capture;
  auto          unique_ptr = std::make_unique<int>(42);
  debug_macro(unique_ptr);
  std::string output = capture.getOutput();

  EXPECT_TRUE(output.find("unique_ptr = 42") != std::string::npos);
}
TEST(DebugMacroAdvancedTest, CustomStructOutput) {
  StdoutCapture capture;
  TestStruct    x;
  debug_macro(x);
  std::string output = capture.getOutput();

  EXPECT_TRUE(output.find("x = TestStruct(a=8, b=2.32)") != std::string::npos);
}
TEST(DebugMacroAdvancedTest, EnumOutput) {
  StdoutCapture capture;
  debug_macro(RED, BLUE);
  std::string output = capture.getOutput();

  EXPECT_TRUE(output.find("RED = 8") != std::string::npos);
  EXPECT_TRUE(output.find("BLUE = 122") != std::string::npos);
}
TEST(DebugMacroAdvancedTest, MultiRankArrayOutput) {
  StdoutCapture capture;
  int           multi_rank_array[][2]{
    {1, 2},
    {3, 4},
  };
  debug_macro(multi_rank_array);
  std::string output = capture.getOutput();

  EXPECT_TRUE(output.find("multi_rank_array = [[1, 2], [3, 4]]") !=
              std::string::npos);
}
