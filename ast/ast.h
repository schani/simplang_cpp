#pragma once
#include <iostream>
#include <memory>

#undef GOOGLE_STRIP_LOG
#define GOOGLE_STRIP_LOG 0
#include <glog/logging.h>

#include "tokens/tokens.h"

namespace simp {
enum class ExpressionType {
  INTEGER,
  IF,
  NOT,
  NEGATIVE,
  PARENTHESIS,
  BINARY,
  LET,
  IDENTIFIER,
  // RECUR,
  // LOOP,
};

class ParsePrintable {
 public:
  virtual std::string to_string(int indent = 0) = 0;
  virtual ~ParsePrintable() {}
  std::string spacing(int indent = 0) {
    std::string result = "";
    for (int i = 0; i < indent; ++i) {
      result += "\t";
    }
    return result;
  }
};
class Expression : public ParsePrintable {
 public:
  Expression(ExpressionType type) : type_(type) {}
  virtual int eval() = 0;

  ExpressionType type() { return type_; }
  virtual ~Expression() {}

 private:
  ExpressionType type_;
};

class IntExpression : public Expression {
 public:
  IntExpression(int value)
      : Expression(ExpressionType::INTEGER), value_(value) {}
  int eval() override { return value_; }

  std::string to_string(int indent = 0) override {
    return spacing(indent) + std::to_string(value_);
  }

 private:
  int value_;
};
class IfExpression : public Expression {
 public:
  IfExpression(std::unique_ptr<KeywordToken> if_token,
               std::unique_ptr<Expression> condition,
               std::unique_ptr<KeywordToken> then_token,
               std::unique_ptr<Expression> consequent,
               std::unique_ptr<KeywordToken> else_token,
               std::unique_ptr<Expression> alternative,
               std::unique_ptr<KeywordToken> end_token)
      : Expression(ExpressionType::IF),
        if_token_(std::move(if_token)),
        condition_(std::move(condition)),
        then_token_(std::move(then_token)),
        consequent_(std::move(consequent)),
        else_token_(std::move(else_token)),
        alternative_(std::move(alternative)),
        end_token_(std::move(end_token)) {}

  std::unique_ptr<KeywordToken>& if_token() { return if_token_; }
  std::unique_ptr<KeywordToken>& then_token() { return then_token_; }
  std::unique_ptr<KeywordToken>& else_token() { return else_token_; }
  std::unique_ptr<Expression>& condition() { return condition_; }
  std::unique_ptr<Expression>& consequent() { return consequent_; }
  std::unique_ptr<Expression>& alternative() { return alternative_; }
  std::unique_ptr<KeywordToken>& end_token() { return end_token_; }

  int eval() override {
    if (condition_->eval()) {
      return consequent_->eval();
    }
    return alternative_->eval();
  }

  std::string to_string(int indent = 0) override {
    return spacing(indent) + "If\n" + condition_->to_string(indent + 1) + "\n" +
           consequent_->to_string(indent + 1) + "\n" +
           alternative_->to_string(indent + 1);
  }

 private:
  std::unique_ptr<KeywordToken> if_token_;
  std::unique_ptr<Expression> condition_;
  std::unique_ptr<KeywordToken> then_token_;
  std::unique_ptr<Expression> consequent_;
  std::unique_ptr<KeywordToken> else_token_;
  std::unique_ptr<Expression> alternative_;
  std::unique_ptr<KeywordToken> end_token_;
};

class BinaryExpression : public Expression {
 public:
  BinaryExpression(std::unique_ptr<Expression> left,
                   std::unique_ptr<Expression> right,
                   std::unique_ptr<OperatorToken> operator_token)
      : Expression(ExpressionType::IF),
        left_(std::move(left)),
        right_(std::move(right)),
        operator_token_(std::move(operator_token)) {}

  std::string to_string(int indent = 0) override {
    return spacing(indent) + op_to_string(operator_token_->op()) + "\n" +
           left_->to_string(indent + 1) + "\n" + right_->to_string(indent + 1);
  }

  int eval() override {
    if (operator_token_->op() == Operator::PLUS) {
      return left_->eval() + right_->eval();
    } else if (operator_token_->op() == Operator::TIMES) {
      return left_->eval() * right_->eval();
    } else if (operator_token_->op() == Operator::LESS_THAN) {
      return left_->eval() < right_->eval();
    } else if (operator_token_->op() == Operator::LOGICAL_AND) {
      return left_->eval() && right_->eval();
    } else if (operator_token_->op() == Operator::LOGICAL_OR) {
      return left_->eval() || right_->eval();
    } else if (operator_token_->op() == Operator::EQUALS) {
      return left_->eval() == right_->eval();
    }
    return 0;
  }

 private:
  std::unique_ptr<Expression> left_;
  std::unique_ptr<Expression> right_;
  std::unique_ptr<OperatorToken> operator_token_;
};

class NotExpression : public Expression {
 public:
  NotExpression(std::unique_ptr<Expression> expression)
      : Expression(ExpressionType::NOT), expression_(std::move(expression)) {}

  std::string to_string(int indent = 0) override {
    return spacing(indent) + "NotExpression:\n" +
           expression_->to_string(indent + 1);
  }

  int eval() override { return !expression_->eval(); }

 private:
  std::unique_ptr<Expression> expression_;
};

class NegativeExpression : public Expression {
 public:
  NegativeExpression(std::unique_ptr<Expression> expression)
      : Expression(ExpressionType::NEGATIVE),
        expression_(std::move(expression)) {}

  std::string to_string(int indent = 0) override {
    return spacing(indent) + "NegativeExpression:\n" +
           expression_->to_string(indent + 1);
  }

  int eval() override { return -expression_->eval(); }

 private:
  std::unique_ptr<Expression> expression_;
};

class ParenthesizedExpression : public Expression {
 public:
  ParenthesizedExpression(std::unique_ptr<OperatorToken> open_paren,
                          std::unique_ptr<Expression> expression,
                          std::unique_ptr<OperatorToken> close_paren)
      : Expression(ExpressionType::PARENTHESIS),
        open_paren_(std::move(open_paren)),
        expression_(std::move(expression)),
        close_paren_(std::move(close_paren)) {
    LOG(INFO) << "ParenthesizedExpression created" << std::endl;
  }

  std::string to_string(int indent = 0) override {
    return spacing(indent) + "(" + expression_->to_string() + ")";
  }
  int eval() override { return expression_->eval(); }

 private:
  std::unique_ptr<OperatorToken> open_paren_;
  std::unique_ptr<Expression> expression_;
  std::unique_ptr<OperatorToken> close_paren_;
};

class Binding : public ParsePrintable {
 public:
  Binding(std::unique_ptr<IdentifierToken> identifier,
          std::unique_ptr<OperatorToken> assign,
          std::unique_ptr<Expression> expression)
      : identifier_(std::move(identifier)),
        assign_(std::move(assign)),
        expression_(std::move(expression)) {
    LOG(INFO) << "Binding created" << std::endl;
  }

  std::unique_ptr<IdentifierToken>& identifier() { return identifier_; }
  std::unique_ptr<OperatorToken>& assign() { return assign_; }
  std::unique_ptr<Expression>& expression() { return expression_; }

  std::string to_string(int indent = 0) {
    return spacing(indent) + identifier_->to_string() + " = " +
           expression_->to_string();
  }

 private:
  std::unique_ptr<IdentifierToken> identifier_;
  std::unique_ptr<OperatorToken> assign_;
  std::unique_ptr<Expression> expression_;
};

class LetExpression : public Expression {
 public:
  LetExpression(
      std::unique_ptr<KeywordToken> let_keyword,
      std::unique_ptr<std::unordered_map<std::string, std::unique_ptr<Binding>>>
          bindings,
      std::unique_ptr<KeywordToken> in_keyword,
      std::unique_ptr<Expression> expression,
      std::unique_ptr<KeywordToken> end_keyword)
      : Expression(ExpressionType::LET),
        let_keyword_(std::move(let_keyword)),
        bindings_(std::move(bindings)),
        in_keyword_(std::move(in_keyword)),
        expression_(std::move(expression)),
        end_keyword_(std::move(end_keyword)) {
    LOG(INFO) << "LetExpression created" << std::endl;
  }

  std::string to_string(int indent = 0) override {
    std::string result = spacing(indent) + "Let\n";
    for (const auto& pair : *bindings_) {
      result += spacing(indent + 1) + pair.first + " = " +
                pair.second->to_string() + "\n";
    }
    result += spacing(indent) + "In\n" + expression_->to_string(indent + 1);
    return result;
  }

  int eval() override {
    return 1;  // TODO: implement eval
  }

  void print_bindings() {
    for (const auto& binding : *bindings_) {
      std::cout << binding.second->to_string() << std::endl;
    }
  }

 private:
  std::unique_ptr<KeywordToken> let_keyword_;
  std::unique_ptr<std::unordered_map<std::string, std::unique_ptr<Binding>>>
      bindings_;
  std::unique_ptr<KeywordToken> in_keyword_;
  std::unique_ptr<Expression> expression_;
  std::unique_ptr<KeywordToken> end_keyword_;
};

class IdentifierExpression : public Expression {
 public:
  IdentifierExpression(std::string name)
      : Expression(ExpressionType::IDENTIFIER), name_(name) {
    LOG(INFO) << "IdentifierExpression created" << std::endl;
  }

  IdentifierExpression(IdentifierToken* identifier_token)
      : Expression(ExpressionType::IDENTIFIER),
        name_(identifier_token->name()) {
    LOG(INFO) << "IdentifierExpression created from pointer" << std::endl;
  }

  std::string to_string(int indent = 0) override {
    return spacing(indent) + name_;
  }

  int eval() override {
    return 1;  // TODO: implement eval
  }

 private:
  std::string name_;
};

class Ast {
 public:
  Ast(std::unique_ptr<Expression> root) : root_(std::move(root)) {}
  int eval() { return root_->eval(); }
  std::unique_ptr<Expression>& root() { return root_; }
  std::string to_string() { return root_->to_string(); }

 private:
  std::unique_ptr<Expression> root_;
};

}  // namespace simp