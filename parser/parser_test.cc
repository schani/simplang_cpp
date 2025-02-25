#include "parser/parser.h"

#include "gtest/gtest.h"
#include "lexer/lexer.h"

namespace simp {
namespace {
using ::std::move;
class ParserTest : public ::testing::Test {
 protected:
  ParserTest() {}
  ~ParserTest() override {}
  void SetUp() override {}
};

TEST_F(ParserTest, ParsesIntExpression) {
  Lexer lexer("examples/just_nums.sl");
  ASSERT_TRUE(lexer.scan());
  Parser parser(move(lexer.tokens()));
  ASSERT_TRUE(parser.parse());
  auto ast = parser.ast();
  EXPECT_EQ(ast->root()->type(), ExpressionType::INTEGER);
}
}  // namespace
}  // namespace simp
