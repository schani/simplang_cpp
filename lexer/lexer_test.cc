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
// /************** Solo operators ********/
// TEST_F(LexerTest, OpenParen) {
//   Lexer lexer{"open_paren.sl"};
//   bool success = lexer.scan();
//   ASSERT_TRUE(success);
//   const auto& tokens = lexer.tokens();
//   // ASSERT_THAT(tokens.size(), Eq(1));
//   // const auto& token = tokens[0];
//   // EXPECT_THAT(token.type, Eq(TokenType::OPERATOR));
//   // EXPECT_THAT(token.to_string(), StrEq("open-paren-operator"));
// }
TEST_F(LexerTest, FailsToOpenNonExistentFile) {
  Lexer lexer{"non_existent_file.sl"};
  EXPECT_THAT(lexer.file_name(), StrEq("non_existent_file.sl"));
  bool success = lexer.scan();
  ASSERT_FALSE(success);
  EXPECT_THAT(lexer.tokens().size(), Eq(0));
}

}  // namespace
}  // namespace simp