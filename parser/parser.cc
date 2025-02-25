#include "parser.h"

#include <memory>

#include "tokens/tokens.h"
namespace simp {

std::unique_ptr<KeywordToken> Lexer::expect_keyword(
    const std::string& keyword) {
  if (token->type() == TokenType::KEYWORD) {
    auto& keyword_token =
        static_cast<std::unique_ptr<KeywordToken>>(token.get());
    if (keyword_token->keyword() == keyword) {
      return keyword_token;
    }
  }
  return nullptr;
}

std::unique_ptr<OperatorToken> Lexer::expect_operator() {
  auto& token = tokens_.front();
  if (token->type() != TokenType::OPERATOR) {
    return nullptr;
  }
  auto& operator_token =
      static_cast<std::unique_ptr<OperatorToken>>(token.get());
  return operator_token;
}

bool Parser::parse() {
  std::unique_ptr<Expression> binary_expression = parse_binary_expression();
  if (!binary_expression) {
    return false;
  }
  ast_ = std::make_unique<Ast>(std::move(binary_expression));
  return true;
}

std::unique_ptr<Expression> Parser::parse_primary_expression() {
  if (tokens_.size() == 0) {
    return nullptr;
  }
  auto& token = tokens_.front();
  tokens_.pop();
  if (token->type() == TokenType::INTEGER) {
    auto& integer_token =
        static_cast<std::unique_ptr<IntegerToken>>(token.get());
    return std::make_unique<IntExpression>(integer_token->value());
  } else if (token->type() == TokenType::KEYWORD) {
    std::unique_ptr<KeywordToken> keyword_token =
        static_cast<std::unique_ptr<KeywordToken>>(token.get());
    if (keyword_token->keyword() == "if") {
      auto condition = parse_binary_expression();
      std::unique_ptr<KeywordToken> then_keyword = expect_keyword("then");
      if (!then_keyword) {
        return nullptr;
      }
      auto consequent = parse_binary_expression();
      std::unique_ptr<KeywordToken> else_keyword = expect_keyword("else");
      if (!else_keyword) {
        return nullptr;
      }
      auto alternative = parse_binary_expression();
      return std::make_unique<IfExpression>(
          std::move(keyword_token), std::move(condition),
          std::move(then_keyword), std::move(consequent),
          std::move(else_keyword), std::move(alternative));
    }
  }  // if keyword
}

std::unique_ptr<Expression> Parser::parse_binary_expression() {
  std::unique_ptr<Expression> left = parse_primary_expression();
  if (!left) {
    return nullptr;
  }
  auto& token = tokens_.front();
  tokens_.pop();
  const auto& op = expect_operator();
  if (!op) {
    return left;
  }
  const auto& operator_token =
      static_cast<std::unique_ptr<simp::OperatorToken>>(token.get());
  const auto& right = parse_binary_expression();
  return std::make_unique<BinaryExpression>(std::move(left), std::move(right),
                                            operator_token->op());
}

}  // namespace simp