#include <gtest/gtest.h>
//
#include <debug_macro.hpp>
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
// Basic type tests
TEST(DebugMacroBasicTest, IntegerOutput) {
  StdoutCapture capture;
  int           x = 42;
  debug_macro(x);
  std::string output = capture.getOutput();

  EXPECT_TRUE(output.find("basic.cpp") != std::string::npos) << output;
  EXPECT_TRUE(output.find("x = 42") != std::string::npos) << output;
}
TEST(DebugMacroBasicTest, StringOutput) {
  StdoutCapture capture;
  std::string   name = "debug_macro";
  debug_macro(name);
  std::string output = capture.getOutput();

  EXPECT_TRUE(output.find("name = \"debug_macro\"") != std::string::npos);
}
TEST(DebugMacroBasicTest, MultipleVariablesOutput) {
  StdoutCapture capture;
  int           x    = 10;
  std::string   name = "test";
  debug_macro(x, name);
  std::string output = capture.getOutput();

  EXPECT_TRUE(output.find("x = 10") != std::string::npos);
  EXPECT_TRUE(output.find("name = \"test\"") != std::string::npos);
}
TEST(DebugMacroBasicTest, ConstAndReferenceVariables) {
  StdoutCapture capture;
  int const     lval = 8;
  int         &&xval = 8;
  debug_macro(lval, (int &&)xval);
  std::string output = capture.getOutput();

  EXPECT_TRUE(output.find("lval = 8") != std::string::npos);
  EXPECT_TRUE(output.find("(int &&)xval = 8") != std::string::npos);
}
TEST(DebugMacroBasicTest, CharOutput) {
  StdoutCapture capture;
  char          c = 'A';
  debug_macro(c);
  std::string output = capture.getOutput();

  EXPECT_TRUE(output.find("c = \'A\'") != std::string::npos);
}
TEST(DebugMacroBasicTest, StringLiteralOutput) {
  StdoutCapture capture;
  debug_macro("hello");
  std::string output = capture.getOutput();

  EXPECT_TRUE(output.find("\"hello\"") != std::string::npos);
}
