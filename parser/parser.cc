#include "parser.h"

#include <memory>

#include "tokens/tokens.h"
namespace simp {

std::unique_ptr<KeywordToken> Parser::expect_keyword(
    const std::string& keyword) {
  std::unique_ptr<Token> token = std::move(tokens_.front());
  tokens_.pop_front(); // Always pop the front token first

  if (!token) {
    LOG(INFO) << "----------expect_keyword \"" << keyword << "\" not found";
    tokens_.push_front(std::move(token));
    return nullptr;
  }

  if (token->type() == TokenType::KEYWORD) {
    LOG(INFO) << "----------expect_keyword keyword found";
    const auto& keyword_token = static_cast<KeywordToken*>(token.get());

    if (keyword_token->keyword() == keyword) {
      LOG(INFO) << "----------expect_keyword \"" << keyword << "\" match found";
      return std::make_unique<KeywordToken>(keyword_token);
    } else {
      LOG(INFO) << "----------expect_keyword actual keyword: "
                << keyword_token->keyword();
      tokens_.push_front(std::move(token));
      return nullptr;
    }
  }
  tokens_.push_front(std::move(token));
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
  LOG(INFO) << "----------expect_binary_operator: operator found";
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

std::unique_ptr<IdentifierToken> Parser::expect_identifier() {
  std::unique_ptr<Token> token = std::move(tokens_.front());
  tokens_.pop_front();
  if (token->type() != TokenType::IDENTIFIER) {
    tokens_.push_front(std::move(
        token));  // needs to be pushed to the front for the algorithm to work.
    return nullptr;
  }
  const auto& identifier_token = static_cast<IdentifierToken*>(token.get());
  return std::make_unique<IdentifierToken>(identifier_token);
}

std::unique_ptr<OperatorToken> Parser::expect_assign_operator() {
  std::unique_ptr<Token> token = std::move(tokens_.front());
  tokens_.pop_front();
  if (token->type() != TokenType::OPERATOR) {
    tokens_.push_front(std::move(
        token));  // needs to be pushed to the front for the algorithm to work.
    return nullptr;
  }
  const auto& operator_token = static_cast<OperatorToken*>(token.get());
  if (operator_token->op() != Operator::ASSIGN) {
    tokens_.push_front(std::move(token));
    return nullptr;
  }
  return std::make_unique<OperatorToken>(operator_token);
}

bool Parser::parse() {
  auto binary_expression = parse_binary_expression();

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
  LOG(INFO) << "-------primary has " << tokens_.size()
            << " tokens left in begining";
  if (tokens_.size() == 0) {
    LOG(INFO) << "-------parse no tokens left";
    return nullptr;
  }

  auto token = std::move(tokens_.front());

  tokens_.pop_front();
  LOG(INFO) << "-------primary token == nullptr: " << (token == nullptr);
  LOG(INFO) << "-------primary popped token and have " << tokens_.size()
            << " tokens left before if statements type";

  if (token->type() == TokenType::INTEGER) {
    LOG(INFO) << "-------primary Parsing integer expression";
    const auto& integer_token_ptr = static_cast<IntegerToken*>(token.get());
    return std::make_unique<IntExpression>(integer_token_ptr->value());
  } else if (token->type() == TokenType::KEYWORD) {
    LOG(INFO) << "-------primary Parsing keyword expression";
    const auto& keyword_token_ptr = static_cast<KeywordToken*>(token.get());
    auto keyword_token = std::make_unique<KeywordToken>(keyword_token_ptr);

    if (keyword_token->keyword() == "if") {
      LOG(INFO) << "-------primary Parsing if expression";
      auto condtion = parse_binary_expression();
      auto then_token = expect_keyword("then");
      if (!then_token) {
        LOG(ERROR) << "Then not found in if statenent";
        return nullptr;
      }
      auto consequent = parse_binary_expression();
      if (!consequent) {
        LOG(ERROR) << "Consequent expression not found in if statenent";
        return nullptr;
      }
      auto else_token = expect_keyword("else");
      if (!else_token) {
        LOG(ERROR) << "Else token not found in if statenent";
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
    } else if (keyword_token->keyword() == "let") {
      LOG(INFO) << "-------primary Parsing let expression";
      auto bindings_map = std::make_unique<
          std::unordered_map<std::string, std::unique_ptr<Binding>>>();
      auto bindings_success = parse_bindings(std::move(bindings_map));
      if (!bindings_success) {
        LOG(ERROR) << "Bindings not found in let statement";
        return nullptr;
      }
      LOG(INFO) << "-------primary Bindings found in let statement";
      auto in_keyword = expect_keyword("in");
      if (!in_keyword) {
        LOG(ERROR) << "-------primary In keyword not found in let statement";
        return nullptr;
      } else {
        LOG(INFO) << "-------primary In keyword found in let statement";
      }
      auto expression = parse_binary_expression();
      if (!expression) {
        LOG(ERROR) << "Expression not found in let statement";
        return nullptr;
      }
      LOG(INFO) << "-------primary Expression found in let statement";
      auto end = expect_keyword("end");
      if (!end) {
        LOG(ERROR) << "End not found in let statement";
        return nullptr;
      }
      LOG(INFO) << "-------primary End found in let statement";
      return std::make_unique<LetExpression>(
          std::move(keyword_token), std::move(bindings_map),
          std::move(in_keyword), std::move(expression), std::move(end));
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
  } else if (token->type() == TokenType::IDENTIFIER) {
    LOG(INFO) << "-------primary Parsing identifier expression";
    const auto& identifier_token_ptr =
        static_cast<IdentifierToken*>(token.get());
    return std::make_unique<IdentifierExpression>(identifier_token_ptr);
  } else {
    LOG(INFO) << "-------primary what is this token";
  }
  LOG(ERROR) << "-------primary expression not regonized";
  return nullptr;
}

bool Parser::parse_bindings(
    std::unique_ptr<std::unordered_map<std::string, std::unique_ptr<Binding>>>
        bindings_map) {
  LOG(INFO) << "*******************Parsing bindings*************";
  LOG(INFO) << "-------bindings has " << tokens_.size()
            << " tokens left in begining";
  if (tokens_.size() == 0) {
    LOG(ERROR) << "No tokens found in parsing bindings";
    return false;
  }
  auto identifier = expect_identifier();
  if (!identifier) {
    LOG(ERROR) << "-------bindings Identifier not found";
    return false;
  }
  LOG(INFO) << "-------bindings identifier found";
  auto assign_operator = expect_assign_operator();
  if (!assign_operator) {
    LOG(ERROR) << "Assign operator not found in bindings";
    return false;
  }
  LOG(INFO) << "-------bindings Assign found";
  auto expression = parse_binary_expression();
  if (!expression) {
    LOG(ERROR) << "-------bindings Expression not found";
    return false;
  }
  LOG(INFO) << "-------bindings Expression found";
  bindings_map->insert(
      {identifier->name(), std::make_unique<Binding>(std::move(identifier),
                                                     std::move(assign_operator),
                                                     std::move(expression))});

  auto and_keyword = expect_keyword("and");
  if (and_keyword) {
    LOG(INFO) << "-------bindings and keyword found";
    return parse_bindings(std::move(bindings_map));
  } else {
    LOG(INFO) << "-------bindings and keyword not found returning true";

    return true;
  }
}

std::unique_ptr<Expression> Parser::parse_binary_expression() {
  LOG(INFO) << "*******************Parsing binary expression*************";
  auto left = parse_primary_expression();
  if (!left) {
    return nullptr;
  }

  if (tokens_.size() == 0) {
    return left;
  }
  auto binary_operator = expect_binary_operator();

  if (!binary_operator) {
    LOG(INFO) << "-------binary No operator found, returning left";
    return left;
  }

  auto right = parse_binary_expression();
  if (!right) {
    LOG(ERROR) << "Found operator but failed to parse right expression";
    return nullptr;
  }
  return std::make_unique<BinaryExpression>(std::move(left), std::move(right),
                                            std::move(binary_operator));
}

}  // namespace simp