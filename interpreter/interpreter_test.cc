#include "interpreter.h"

#include "gtest/gtest.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

namespace simp {
namespace {
class InterpreterTest : public ::testing::Test {
 protected:
  InterpreterTest() {}
  ~InterpreterTest() override {}
  void SetUp() override {}
};

}  // namespace
}  // namespace simp
