#include "ast.h"

#include "gtest/gtest.h"

namespace simp {
namespace {
class AstTest : public ::testing::Test {
 protected:
  AstTest() {}
  ~AstTest() override {}
  void SetUp() override {}
};

TEST_F(AstTest, CreatesIntExpression) {
  IntExpression int_expr(42);
  EXPECT_EQ(int_expr.eval(), 42);
  EXPECT_EQ(int_expr.to_string(), "42");
}

TEST_F(AstTest, CreatesIfExpression) {
  std::unique_ptr<KeywordToken> if_token =
      std::make_unique<KeywordToken>("if", 0, 0, "test");
  std::unique_ptr<KeywordToken> then_token =
      std::make_unique<KeywordToken>("then", 0, 0, "test");
  std::unique_ptr<KeywordToken> else_token =
      std::make_unique<KeywordToken>("then", 0, 0, "test");
  std::unique_ptr<IntExpression> condition = std::make_unique<IntExpression>(1);
  std::unique_ptr<IntExpression> consequent_expr =
      std::make_unique<IntExpression>(2);
  std::unique_ptr<IntExpression> alternative_expr =
      std::make_unique<IntExpression>(3);
  IfExpression if_expr(std::move(if_token), std::move(condition),
                       std::move(then_token), std::move(consequent_expr),
                       std::move(else_token), std::move(alternative_expr));
  EXPECT_EQ(if_expr.to_string(), "If\ncondition:1\nthen:2\nelse:3");
  EXPECT_EQ(if_expr.condition()->eval(), 1);
  EXPECT_EQ(if_expr.consequent()->eval(), 2);
  EXPECT_EQ(if_expr.alternative()->eval(), 3);
  EXPECT_EQ(if_expr.eval(), 2);
}

TEST_F(AstTest, CreatesAst) {
  std::unique_ptr<IntExpression> root = std::make_unique<IntExpression>(1);
  Ast ast(std::move(root));
  EXPECT_EQ(ast.root()->eval(), 1);
}

}  // namespace
}  // namespace simp
