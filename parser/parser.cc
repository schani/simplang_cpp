#include "parser.h"

#include <memory>

#include "tokens/tokens.h"
#define DEBUG
namespace simp {

std::unique_ptr<KeywordToken> Parser::expect_keyword(
    const std::string& keyword) {
  std::unique_ptr<Token> token = std::move(tokens_.front());
  if (!token) {
#ifdef DEBUG
    std::cout << "----------expect_keyword No token found. token == nullptr:"
              << (token == nullptr) << std::endl;
#endif
    return nullptr;
  }
  if (token->type() == TokenType::KEYWORD) {
#ifdef DEBUG
    std::cout << "----------expect_keyword Keyword found" << std::endl;
#endif
    const auto& keyword_token = static_cast<KeywordToken*>(token.get());

    if (keyword_token->keyword() == keyword) {
      tokens_.pop_front();
      return std::make_unique<KeywordToken>(keyword_token);
    }
  }
  return nullptr;
}

std::unique_ptr<OperatorToken> Parser::expect_operator() {
  std::unique_ptr<Token> token = std::move(tokens_.front());
  tokens_.pop_front();
  if (token->type() != TokenType::OPERATOR) {
    tokens_.push_front(std::move(
        token));  // needs to be pushed to the front for the algorithm to work.
    return nullptr;
  }
#ifdef DEBUG
  std::cout << "----------expect_operator: operator found" << std::endl;
#endif
  const auto& operator_token = static_cast<OperatorToken*>(token.get());
  tokens_.pop_front();
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
  std::cout << "-------primary " << tokens_.size() << " tokens left in begining"
            << std::endl;
#endif
  if (tokens_.size() == 0) {
    return nullptr;
  }

  auto token = std::move(tokens_.front());
  tokens_.pop_front();
#ifdef DEBUG
  std::cout << "-------primary popped token and have " << tokens_.size()
            << " tokens left before if statements" << std::endl;
#endif

  if (token->type() == TokenType::INTEGER) {
#ifdef DEBUG
    std::cout << "-------primary Parsing integer expression" << std::endl;
#endif
    const auto& integer_token_ptr = static_cast<IntegerToken*>(token.get());
    return std::make_unique<IntExpression>(integer_token_ptr->value());
  } else if (token->type() == TokenType::KEYWORD) {
#ifdef DEBUG
    std::cout << "-------primary Parsing keyword expression" << std::endl;
#endif
    const auto& keyword_token_ptr = static_cast<KeywordToken*>(token.get());
    auto keyword_token = std::make_unique<KeywordToken>(keyword_token_ptr);

    if (keyword_token->keyword() == "if") {
      auto condtion = parse_binary_expression();
      auto then_token = expect_keyword("then");
      if (!then_token) {
#ifdef DEBUG
        std::cout << "Token not found in if statenent" << std::endl;
#endif
        return nullptr;
      }
      auto consequent = parse_binary_expression();
      if (!consequent) {
#ifdef DEBUG
        std::cout << "Consequent expression not found in if statenent"
                  << std::endl;
#endif
        return nullptr;
      }
      auto else_token = expect_keyword("else");
      if (!else_token) {
#ifdef DEBUG
        std::cout << "Else token not found in if statenent" << std::endl;
#endif
        return nullptr;
      }
      auto alternative = parse_binary_expression();
      if (!alternative) {
#ifdef DEBUG
        std::cout << "Alternative expression not found in if statenent"
                  << std::endl;
#endif
        return nullptr;
      }
      auto end_token = expect_keyword("end");
      if (!end_token) {
#ifdef DEBUG
        std::cout << "End token not found in if statenent" << std::endl;
#endif
        return nullptr;
      }
      return std::make_unique<IfExpression>(
          std::move(keyword_token), std::move(condtion), std::move(then_token),
          std::move(consequent), std::move(else_token), std::move(alternative),
          std::move(end_token));
#ifdef DEBUG
#endif
    }
  }
  std::cout << "Expression not recognized" << std::endl;
  return nullptr;
}

std::unique_ptr<Expression> Parser::parse_binary_expression() {
#ifdef DEBUG
  std::cout << "*******************Parsing binary expression*************"
            << std::endl;
#endif
  auto left = parse_primary_expression();
  if (!left) {
    return nullptr;
  }

  if (tokens_.size() == 0) {
    return left;
  }
  auto op = expect_operator();

  if (!op) {
#ifdef DEBUG
    std::cout << "-------binary No operator found, returning left" << std::endl;
#endif
    return left;
  }

  const auto& operator_token = static_cast<OperatorToken*>(op.get());
  auto right = parse_binary_expression();
  if (!right) {
#ifdef DEBUG
    std::cerr << "Found operator but failed to parse right expression"
              << std::endl;
#endif
    return nullptr;
  }
  return std::make_unique<BinaryExpression>(std::move(left), std::move(right),
                                            operator_token->op());
}

}  // namespace simp