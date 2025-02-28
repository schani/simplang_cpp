#include "parser.h"

#include <memory>

#include "tokens/tokens.h"
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

std::unique_ptr<OperatorToken> Parser::expect_binary_operator() {
  std::unique_ptr<Token> token = std::move(tokens_.front());
  tokens_.pop_front();
  if (token->type() != TokenType::OPERATOR) {
    tokens_.push_front(std::move(
        token));  // needs to be pushed to the front for the algorithm to work.
    return nullptr;
  }
#ifdef DEBUG
  std::cout << "----------expect_binary_operator: operator found" << std::endl;
#endif
  const auto& operator_token = static_cast<OperatorToken*>(token.get());
  if (!operator_token->is_binary()) {
    tokens_.push_front(std::move(token));
    return nullptr;
  }
  return std::make_unique<OperatorToken>(operator_token);
}
std::unique_ptr<OperatorToken> Parser::expect_close_paren() {
  std::unique_ptr<Token> token = std::move(tokens_.front());
  tokens_.pop_front();
  if (token->type() != TokenType::OPERATOR) {
    tokens_.push_front(std::move(
        token));  // needs to be pushed to the front for the algorithm to work.
    return nullptr;
  }
  const auto& operator_token = static_cast<OperatorToken*>(token.get());
  if (operator_token->op() != Operator::CLOSE_PAREN) {
    tokens_.push_front(std::move(token));
    return nullptr;
  }
  return std::make_unique<OperatorToken>(operator_token);
}

bool Parser::parse() {
  auto binary_expression = parse_binary_expression();

  LOG(INFO) << "-------parse expression to_string:"
            << binary_expression->to_string();
  if (!binary_expression) {
    LOG(ERROR) << "-------parse Failed to parse binary expression";
    return false;
  }
  LOG(INFO) << "-------parse Parsed binary expression, making ast";
  ast_ = std::make_unique<Ast>(std::move(binary_expression));
  return true;
}

std::unique_ptr<Expression> Parser::parse_primary_expression() {
  LOG(INFO) << "*********Parsing primary expression*******" << std::endl;
  LOG(INFO) << "-------parse has " << tokens_.size()
            << " tokens left in begining";
  if (tokens_.size() == 0) {
    LOG(INFO) << "-------parse no tokens left";
    return nullptr;
  }

  auto token = std::move(tokens_.front());
  tokens_.pop_front();
  LOG(INFO) << "-------primary popped token and have " << tokens_.size()
            << " tokens left before if statements";

  if (token->type() == TokenType::INTEGER) {
    LOG(INFO) << "-------primary Parsing integer expression";
    const auto& integer_token_ptr = static_cast<IntegerToken*>(token.get());
    return std::make_unique<IntExpression>(integer_token_ptr->value());
  } else if (token->type() == TokenType::KEYWORD) {
    LOG(INFO) << "-------primary Parsing keyword expression" << std::endl;
    const auto& keyword_token_ptr = static_cast<KeywordToken*>(token.get());
    auto keyword_token = std::make_unique<KeywordToken>(keyword_token_ptr);

    if (keyword_token->keyword() == "if") {
      auto condtion = parse_binary_expression();
      auto then_token = expect_keyword("then");
      if (!then_token) {
        LOG(ERROR) << "Then not found in if statenent";
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
        LOG(ERROR) << "Alternative expression not found in if statenent";
        return nullptr;
      }
      auto end_token = expect_keyword("end");
      if (!end_token) {
        LOG(ERROR) << "End token not found in if statenent";
        return nullptr;
      }
      return std::make_unique<IfExpression>(
          std::move(keyword_token), std::move(condtion), std::move(then_token),
          std::move(consequent), std::move(else_token), std::move(alternative),
          std::move(end_token));
#ifdef DEBUG
#endif
    }
  } else if (token->type() == TokenType::OPERATOR) {
    LOG(INFO) << "-------primary Parsing operator expression";
    const auto& operator_token_ptr = static_cast<OperatorToken*>(token.get());
    auto operator_token = std::make_unique<OperatorToken>(operator_token_ptr);
    if (operator_token->op() == Operator::CLOSE_PAREN) {
      tokens_.push_front(std::move(operator_token));
      return nullptr;
    } else if (operator_token->op() == Operator::UNARY_MINUS) {
      auto expression = parse_primary_expression();
      if (!expression) {
        LOG(ERROR) << "Expression not recognized for not expression";
        return nullptr;
      }
      return std::make_unique<NegativeExpression>(std::move(expression));
    } else if (operator_token->op() == Operator::NOT) {
      auto expression = parse_primary_expression();
      if (!expression) {
        LOG(ERROR) << "Expression not recognized";
        return nullptr;
      }
      return std::make_unique<NotExpression>(std::move(expression));
    } else if (operator_token->op() == Operator::OPEN_PAREN) {
      auto expression =
          parse_binary_expression();  // this is eating up the last close paren

      auto close_paren = expect_close_paren();

      if (!close_paren) {
        LOG(ERROR) << "Close paren not found";
        return nullptr;
      }
      LOG(INFO) << "-------primary found close paren";
      return std::make_unique<ParenthesizedExpression>(
          std::move(operator_token), std::move(expression),
          std::move(close_paren));
    }
  }
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
  auto binary_operator = expect_binary_operator();

  if (!binary_operator) {
#ifdef DEBUG
    std::cout << "-------binary No operator found, returning left" << std::endl;
#endif
    return left;
  }

  auto right = parse_binary_expression();
  if (!right) {
#ifdef DEBUG
    std::cerr << "Found operator but failed to parse right expression"
              << std::endl;
#endif
    return nullptr;
  }
  return std::make_unique<BinaryExpression>(std::move(left), std::move(right),
                                            std::move(binary_operator));
}

}  // namespace simp