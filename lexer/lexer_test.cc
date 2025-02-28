#include "lexer/lexer.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "tokens/tokens.h"

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

TEST_F(LexerTest, FailsToOpenNonExistentFile) {
  Lexer lexer{"non_existent_file.sl"};
  EXPECT_THAT(lexer.file_name(), StrEq("non_existent_file.sl"));
  bool success = lexer.scan();
  ASSERT_FALSE(success);
  EXPECT_THAT(lexer.tokens().size(), Eq(0));
}

// /************** Solo operators ********/

TEST_F(LexerTest, OpenParen) {
  Lexer lexer{"examples/open_paren.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/open_paren.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  auto token = std::move(std::move(tokens.front()));
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("open-paren-operator"));
}

TEST_F(LexerTest, CloseParen) {
  Lexer lexer{"examples/close_paren.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/close_paren.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  auto token = std::move(tokens.front());
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("close-paren-operator"));
}

TEST_F(LexerTest, Plus) {
  Lexer lexer{"examples/plus.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/plus.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  auto token = std::move(tokens.front());
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("plus-operator"));
}

TEST_F(LexerTest, Times) {
  Lexer lexer{"examples/times.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/times.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  auto token = std::move(tokens.front());
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("times-operator"));
}

TEST_F(LexerTest, UnaryMinus) {
  Lexer lexer{"examples/unary_minus.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/unary_minus.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  auto token = std::move(tokens.front());
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("unary-minus-operator"));
}

TEST_F(LexerTest, Not) {
  Lexer lexer{"examples/not.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/not.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  auto token = std::move(tokens.front());
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("not-operator"));
}

TEST_F(LexerTest, LessThan) {
  Lexer lexer{"examples/less_than.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/less_than.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  auto token = std::move(tokens.front());
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("less-than-operator"));
}

// /************** Binary operators ********/

TEST_F(LexerTest, LogicalAnd) {
  Lexer lexer{"examples/logical_and.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/logical_and.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  auto token = std::move(tokens.front());
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("logical-and-operator"));
}

TEST_F(LexerTest, LogicalOr) {
  Lexer lexer{"examples/logical_or.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/logical_or.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  auto token = std::move(tokens.front());
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("logical-or-operator"));
}

TEST_F(LexerTest, Equals) {
  Lexer lexer{"examples/equals.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/equals.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  auto token = std::move(tokens.front());
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("equals-operator"));
}

TEST_F(LexerTest, Assign) {
  Lexer lexer{"examples/assign.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/assign.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  auto token = std::move(tokens.front());
  EXPECT_THAT(token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token->to_string(), StrEq("assign-operator"));
}

TEST_F(LexerTest, Integer) {
  Lexer lexer("examples/just_nums.sl");
  ASSERT_THAT(lexer.file_name(), StrEq("examples/just_nums.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  auto token = std::move(tokens.front());
  EXPECT_THAT(token->type(), Eq(TokenType::INTEGER));
  EXPECT_THAT(token->to_string(), StrEq("1234567890"));
}

TEST_F(LexerTest, Keyword) {
  for (const auto& pair : Keywords) {
    std::string keyword = pair.first;
    std::string file = "examples/" + keyword + "_keyword.sl";
    Lexer lexer(file);
    ASSERT_THAT(lexer.file_name(), StrEq(file));
    bool success = lexer.scan();
    ASSERT_TRUE(success);
    auto& tokens = lexer.tokens();
    ASSERT_THAT(tokens.size(), Eq(1));
    auto token = std::move(tokens.front());
    EXPECT_THAT(token->type(), Eq(TokenType::KEYWORD));
    EXPECT_THAT(token->to_string(), StrEq(keyword + "-keyword"));
  };
}

TEST_F(LexerTest, Identifier) {
  Lexer lexer("examples/identifier.sl");
  ASSERT_THAT(lexer.file_name(), StrEq("examples/identifier.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(1));
  auto token = std::move(tokens.front());
  EXPECT_THAT(token->type(), Eq(TokenType::IDENTIFIER));
  EXPECT_THAT(token->to_string(), StrEq("qwertyuiop_1234567890"));
}

TEST_F(LexerTest, Add) {
  Lexer lexer("examples/add.sl");
  ASSERT_THAT(lexer.file_name(), StrEq("examples/add.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(9));
  auto let_token = std::move(tokens.front());
  EXPECT_THAT(let_token->type(), Eq(TokenType::KEYWORD));
  EXPECT_THAT(let_token->to_string(), StrEq("let-keyword"));
  tokens.pop_front();
  auto main_token = std::move(tokens.front());
  EXPECT_THAT(main_token->type(), Eq(TokenType::IDENTIFIER));
  EXPECT_THAT(main_token->to_string(), StrEq("main"));
  tokens.pop_front();
  auto a_token = std::move(tokens.front());
  EXPECT_THAT(a_token->type(), Eq(TokenType::IDENTIFIER));
  EXPECT_THAT(a_token->to_string(), StrEq("a"));
  tokens.pop_front();
  auto b_token = std::move(tokens.front());
  EXPECT_THAT(b_token->type(), Eq(TokenType::IDENTIFIER));
  EXPECT_THAT(b_token->to_string(), StrEq("b"));
  tokens.pop_front();
  auto assign_token = std::move(tokens.front());
  EXPECT_THAT(assign_token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(assign_token->to_string(), StrEq("assign-operator"));
  tokens.pop_front();
  auto second_a_token = std::move(tokens.front());
  EXPECT_THAT(second_a_token->type(), Eq(TokenType::IDENTIFIER));
  EXPECT_THAT(second_a_token->to_string(), StrEq("a"));
  tokens.pop_front();
  auto plus_token = std::move(tokens.front());
  EXPECT_THAT(plus_token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(plus_token->to_string(), StrEq("plus-operator"));
  tokens.pop_front();
  auto second_b_token = std::move(tokens.front());
  EXPECT_THAT(second_b_token->type(), Eq(TokenType::IDENTIFIER));
  EXPECT_THAT(second_b_token->to_string(), StrEq("b"));
  tokens.pop_front();
  auto end_token = std::move(tokens.front());
  EXPECT_THAT(end_token->type(), Eq(TokenType::KEYWORD));
  EXPECT_THAT(end_token->to_string(), StrEq("end-keyword"));
}

TEST_F(LexerTest, ShiftL) {
  Lexer lexer("examples/shiftl.sl");
  ASSERT_THAT(lexer.file_name(), StrEq("examples/shiftl.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(48));
}

TEST_F(LexerTest, NotExpression) {
  Lexer lexer{"examples/not_expression.sl"};
  ASSERT_THAT(lexer.file_name(), StrEq("examples/not_expression.sl"));
  bool success = lexer.scan();
  ASSERT_TRUE(success);
  auto& tokens = lexer.tokens();
  ASSERT_THAT(tokens.size(), Eq(2));
  auto not_token = std::move(tokens.front());
  EXPECT_THAT(not_token->type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(not_token->to_string(), StrEq("not-operator"));
  tokens.pop_front();
  auto integer_token = std::move(tokens.front());
  EXPECT_THAT(integer_token->type(), Eq(TokenType::INTEGER));
  EXPECT_THAT(integer_token->to_string(), StrEq("1"));
}

}  // namespace
}  // namespace simp