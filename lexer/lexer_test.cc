#include "lexer/lexer.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "tokens.h"

namespace simp {
namespace {
using ::std::string;
using ::testing::Eq;
using ::testing::StrEq;
class LexerTest : public ::testing::Test {
 protected:
  LexerTest() {}
  ~LexerTest() override {}
  void SetUp() override {}
};

// TEST_F(LexerTest, FailsToOpenNonExistentFile) {
//   Lexer lexer{"non_existent_file.sl"};
//   EXPECT_THAT(lexer.file_name(), StrEq("non_existent_file.sl"));
//   bool success = lexer.scan();
//   ASSERT_FALSE(success);
//   EXPECT_THAT(lexer.tokens().size(), Eq(0));
// }

/************** Solo operators ********/

TEST_F(LexerTest, OpenParen) {
  Lexer lexer{"examples/open_paren.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/open_paren.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  const auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  const auto& token = tokens[0];
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("open-paren-operator"));
}

TEST_F(LexerTest, CloseParen) {
  Lexer lexer{"examples/close_paren.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/close_paren.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  const auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  const auto& token = tokens[0];
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("close-paren-operator"));
}

TEST_F(LexerTest, Plus) {
  Lexer lexer{"examples/plus.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/plus.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  const auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  const auto& token = tokens[0];
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("plus-operator"));
}

TEST_F(LexerTest, Times) {
  Lexer lexer{"examples/times.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/times.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  const auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  const auto& token = tokens[0];
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("times-operator"));
}

TEST_F(LexerTest, UnaryMinus) {
  Lexer lexer{"examples/unary_minus.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/unary_minus.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  const auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  const auto& token = tokens[0];
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("unary-minus-operator"));
}

TEST_F(LexerTest, Not) {
  Lexer lexer{"examples/not.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/not.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  const auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  const auto& token = tokens[0];
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("not-operator"));
}

TEST_F(LexerTest, LessThan) {
  Lexer lexer{"examples/less_than.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/less_than.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  const auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  const auto& token = tokens[0];
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("less-than-operator"));
}

/************** Binary operators ********/

TEST_F(LexerTest, LogicalAnd) {
  Lexer lexer{"examples/logical_and.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/logical_and.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  const auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  const auto& token = tokens[0];
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("logical-and-operator"));
}

TEST_F(LexerTest, LogicalOr) {
  Lexer lexer{"examples/logical_or.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/logical_or.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  const auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  const auto& token = tokens[0];
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("logical-or-operator"));
}

TEST_F(LexerTest, Equals) {
  Lexer lexer{"examples/equals.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/equals.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  const auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  const auto& token = tokens[0];
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("equals-operator"));
}

TEST_F(LexerTest, Assign) {
  Lexer lexer{"examples/assign.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/assign.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  const auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  const auto& token = tokens[0];
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("assign-operator"));
}

TEST_F(LexerTest, Integer) {
  Lexer lexer("examples/just_nums.sl");
  ASSERT_THAT(lexer.file_name(), StrEq("examples/just_nums.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  const auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  const auto& token = tokens[0];
  EXPECT_THAT(token->type(), Eq(TokenType::INTEGER));
  EXPECT_THAT(token->to_string(), StrEq("1234567890"));
}

}  // namespace
}  // namespace simp