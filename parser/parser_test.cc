#include "parser/parser.h"

#include "gtest/gtest.h"
#include "lexer/lexer.h"

namespace simp {
namespace {
class ParserTest : public ::testing::Test {
 protected:
  ParserTest() {}
  ~ParserTest() override {}
  void SetUp() override {}
};

TEST_F(ParserTest, ParsesIntExpression) {
  Lexer lexer("examples/just_nums.sl");
  bool lexer_worked = lexer.scan();
  ASSERT_TRUE(lexer_worked);
  Parser parser(std::move(lexer.tokens()));
  bool parser_worked = parser.parse();
  ASSERT_TRUE(parser_worked);

  auto ast = parser.ast();
  EXPECT_EQ(ast->root()->type(), ExpressionType::INTEGER);
  EXPECT_EQ(ast->root()->eval(), 1234567890);
}

// TEST_F(ParserTest, ParsesIfExpression) {
//   Lexer lexer("examples/if_statement.sl");
//   bool lexer_worked = lexer.scan();
//   ASSERT_TRUE(lexer_worked);
//   Parser parser(std::move(lexer.tokens()));
//   bool parser_worked = parser.parse();
//   ASSERT_TRUE(parser_worked);

//   auto ast = parser.ast();
//   EXPECT_EQ(ast->root()->type(), ExpressionType::IF);
//   EXPECT_TRUE(ast->root()->eval());
// }

}  // namespace
}  // namespace simp
