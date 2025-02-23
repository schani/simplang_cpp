#include "tokens.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace simp {
namespace {
using ::testing::Eq;
using ::testing::StrEq;
class TokensTest : public ::testing::Test {
 protected:
  TokensTest() {}
  ~TokensTest() override {}
  void SetUp() override {}
};

TEST_F(TokensTest, CanCreateOperaatorToken) {
  OperatorToken token(Operator::PLUS, 1, 1, "test");
  EXPECT_THAT(token.type(), Eq(TokenType::OPERATOR));
  EXPECT_THAT(token.op(), Eq(Operator::PLUS));
  EXPECT_THAT(token.to_string(), StrEq("plus-operator"));
  EXPECT_THAT(token.location(),
              StrEq(" in file:\"test\"\ton line:1\tat position:1"));
}

TEST_F(TokensTest, CanCreateIntegerToken) {
  IntegerToken token(123456, 1, 1, "test");
  EXPECT_THAT(token.type(), Eq(TokenType::INTEGER));
  EXPECT_THAT(token.value(), Eq(123456));
  EXPECT_THAT(token.to_string(), StrEq("123456"));
  EXPECT_THAT(token.location(),
              StrEq(" in file:\"test\"\ton line:1\tat position:1"));
}

TEST_F(TokensTest, CanCreateKeywordToken) {
  KeywordToken token("let", 1, 1, "test");
  EXPECT_THAT(token.type(), Eq(TokenType::KEYWORD));
  EXPECT_THAT(token.keyword(), Eq("let"));
  EXPECT_THAT(token.to_string(), StrEq("let-keyword"));
  EXPECT_THAT(token.location(),
              StrEq(" in file:\"test\"\ton line:1\tat position:1"));
}

TEST_F(TokensTest, CanCreateIdentifierToken) {
  IdentifierToken token("qwerttyuiop", 1, 1, "test");
  EXPECT_THAT(token.type(), Eq(TokenType::IDENTIFIER));
  EXPECT_THAT(token.name(), Eq("qwerttyuiop"));
  EXPECT_THAT(token.to_string(), StrEq("qwerttyuiop"));
  EXPECT_THAT(token.location(),
              StrEq(" in file:\"test\"\ton line:1\tat position:1"));
}

}  // namespace
}  // namespace simp