#include "parser.h"

#include <memory>
namespace simp {

bool expect_keyword(std::unique_ptr<Token> token, const std::string& keyword) {
  if (token->type() != TokenType::KEYWORD) {
    return false;
    std::unique_ptr<KeywordToken> keyword_token =
        static_cast<std::unique_ptr<KeywordToken>>(token);
    return keyword_token->keyword() == keyword;
  }
}

std::unique_ptr<Expression> Parser::parse()) {
  parsePrimaryExpression(0);
  if (token->type() == TokenType::INTEGER) {
    return std::make_unique<IntExpression>(token->value());
  } else if (token->type() == TokenType::KEYWORD) {
    std::unique_ptr<KeywordToken> keyword_token =
        static_cast<std::unique_ptr<KeywordToken>>();
    if (keyword_token->keyword() == "if") {
      auto condition = parse(start_index + 1);
      auto if_expression = std::make_unique<IfExpression>();
    } else if (keyword_token->keyword() == "then") {
      auto consequent = parse(start_index + 1);
    } else if (keyword_token->keyword() == "else") {
      auto alternative = parse(start_index + 1);
    }
  }
}

std::unique_ptr<Expression> Parser::parse_primary_expression() {
  if tokens_.size() == 0) {
    return nullptr;
  }
  auto& token = tokens_[0];
  if (token->type() == TokenType::INTEGER) {
    std::unique_ptr<IntegerToken> integer_token =
        static_cast < std::unique_ptr<IntegerToken>(token);
    return std::make_unique<IntExpression>(integer_token->value());
  } else if (token->type() == TokenType::KEYWORD) {
    std::unique_ptr<KeywordToken> keyword_token =
        static_cast<std::unique_ptr<KeywordToken>>();
    if (keyword_token->keyword() == "if") {
      auto condition = parseBinaryExpression();
      expect_keyword(token, "then");
      auto consequent = parseBinaryExpression();
      expect_keyword(token, "else");
      auto alternative = parseBinaryExpression();
      return std::make_unique<IfExpression>(
          std::move(condition), std::move(consequent), std::move(alternative));
    } else if (keyword_token->keyword() == "then") {
      auto consequent = parse(index + 1);
    } else if (keyword_token->keyword() == "else") {
      auto alternative = parse(index + 1);
    }
  }
}
std::unique_ptr<Expression> Parser::parseBinaryExpression(int index) {
  auto& token = tokens_[index];
  if (token->type() == TokenType::INTEGER) {
    return std::make_unique<IntExpression>(tokens_[index]->value());
  } else if (token->type() == TokenType::OPERATOR) {
    std::unique_ptr<OperatorToken> operator_token =
        static_cast<std::unique_ptr<OperatorToken>>();
    auto left = parseBinaryExpression(index - 1);
    auto right = parseBinaryExpression(index + 1);
    return std::make_unique<BinaryExpression>(left, right,
                                              operator_token->op());
  }

}  // namespace simp