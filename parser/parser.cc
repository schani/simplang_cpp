#include "parser.h"

#include <memory>

#include "tokens/tokens.h"
#define DEBUG
namespace simp {

std::unique_ptr<KeywordToken> Parser::expect_keyword(
    const std::string& keyword) {
  auto& token = tokens_.front();
  if (!token) {
    return nullptr;
  }
  tokens_.pop();
  if (token->type() == TokenType::KEYWORD) {
    const auto& keyword_token = static_cast<KeywordToken*>(token.get());
    if (keyword_token->keyword() == keyword) {
      return std::make_unique<KeywordToken>(keyword_token);
    }
  }
  return nullptr;
}

std::unique_ptr<OperatorToken> Parser::expect_operator() {
  auto& token = tokens_.front();
  if (token->type() != TokenType::OPERATOR) {
    return nullptr;
  }
  const auto& operator_token = static_cast<OperatorToken*>(token.get());
  return std::make_unique<OperatorToken>(operator_token);
}

bool Parser::parse() {
#ifdef DEBUG
  std::cout << "**************Parsing***************" << std::endl;
#endif
  auto binary_expression = parse_binary_expression();

#ifdef DEBUG
  std::cout << "-------parse expression to_string:"
            << binary_expression->to_string() << std::endl;
#endif
  if (!binary_expression) {
#ifdef DEBUG
    std::cout << "-------parse Failed to parse binary expression" << std::endl;
#endif
    return false;
  }
#ifdef DEBUG
  std::cout << "-------parse Parsed binary expression, making ast" << std::endl;
#endif
  ast_ = std::make_unique<Ast>(std::move(binary_expression));
  return true;
}

std::unique_ptr<Expression> Parser::parse_primary_expression() {
#ifdef DEBUG
  std::cout << "*********Parsing primary expression*******" << std::endl;
#endif
  if (tokens_.size() == 0) {
#ifdef DEBUG
    std::cout << "No tokens to parse" << std::endl;
#endif
    return nullptr;
  }
#ifdef DEBUG
  std::cout << "-------primary " << tokens_.size()
            << " token(s) in parse in primary" << std::endl;
#endif
  auto& token = tokens_.front();

  std::cout << "-------primary Token type:" << token->type()
            << "\ttoken->to_string():" << token->to_string() << std::endl;
  if (token->type() == TokenType::INTEGER) {
#ifdef DEBUG
    std::cout << "-------primary Parsing integer expression" << std::endl;
#endif
    const auto& integer_token_ptr = static_cast<IntegerToken*>(token.get());
    std::cout << "-------primary Integer value:" << integer_token_ptr->value()
              << std::endl;
    return std::make_unique<IntExpression>(integer_token_ptr->value());
  } else if (token->type() == TokenType::KEYWORD) {
#ifdef DEBUG
    std::cout << "-------primary Parsing keyword expression" << std::endl;
#endif
  }
#ifdef DEBUG
  std::cout << "Expression not recognized" << std::endl;
#endif
  return nullptr;
}

std::unique_ptr<Expression> Parser::parse_binary_expression() {
#ifdef DEBUG
  std::cout << "*******************Parsing binary expression*************"
            << std::endl;
#endif
  auto left = parse_primary_expression();
#ifdef DEBUG
  std::cout << "-------binary left expression to_string:" << left->to_string()
            << std::endl;
#endif
  if (!left) {
#ifdef DEBUG
    std::cout << "Failed to parse left expression" << std::endl;
#endif
    return nullptr;
  }
  if (tokens_.size() == 0) {
#ifdef DEBUG
    std::cout << "-------binary No tokens left to parse" << std::endl;
#endif
    return left;
  }
  const auto& op = expect_operator();
  if (op) {
    std::cout << "-------binary Operator:" << op->to_string() << std::endl;
  } else {
#ifdef DEBUG
    std::cout << "-------binary No operator" << std::endl;
#endif
    return left;
  }
#ifdef DEBUG
  std::cout << "-------binary Parse binary operator:" << op->to_string()
            << std::endl;
#endif
  // auto& operator_token =
  // static_cast<std::unique_ptr<OperatorToken>>(token.get());
  // auto& right = parse_binary_expression();
  // return std::make_unique<BinaryExpression>(std::move(left),
  // std::move(right), operator_token->op());
  return left;
}

}  // namespace simp