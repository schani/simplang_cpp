#pragma once
#include <iostream>
#include <memory>

#include "tokens/tokens.h"

namespace simp {
enum class ExpressionType {
  INTEGER,
  IF,
  BINARY,
  // IN,
  // LET,
  // PLUS,
  // MINUS,
  // TIMES,
  // LESS_THAN,
  // RECUR,
  // LOOP,
  // AND,
  // END,
};
class Expression {
 public:
  Expression(ExpressionType type) : type_(type) {}
  virtual int eval() = 0;
  virtual std::string to_string(int indent = 0) = 0;

  ExpressionType type() { return type_; }

  std::string spacing(int indent = 0) {
    std::string result = "";
    for (int i = 0; i < indent; ++i) {
      result += "\t";
    }
    return result;
  }

 private:
  ExpressionType type_;
};

class IntExpression : public Expression {
 public:
  IntExpression(int value)
      : Expression(ExpressionType::INTEGER), value_(value) {}
  int eval() override { return value_; }

  std::string to_string(int indent) override {
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

  std::string to_string(int indent) override {
    return spacing(indent) + "If\n" + condition_->to_string(indent + 1) + "\n" +
           consequent_->to_string(indent + 1) + "\n" +
           alternative_->to_string(indent + 1);
  }

 private:
  std::unique_ptr<KeywordToken> if_token_;
  std::unique_ptr<KeywordToken> then_token_;
  std::unique_ptr<KeywordToken> else_token_;
  std::unique_ptr<Expression> condition_;
  std::unique_ptr<Expression> consequent_;
  std::unique_ptr<Expression> alternative_;
  std::unique_ptr<KeywordToken> end_token_;
};

class BinaryExpression : public Expression {
 public:
  BinaryExpression(std::unique_ptr<Expression> left,
                   std::unique_ptr<Expression> right, Operator op)
      : Expression(ExpressionType::IF),
        left_(std::move(left)),
        right_(std::move(right)),
        op_(op) {}

  std::string to_string(int indent) override {
    return spacing(indent) + "BinaryExpression:\n" +
           left_->to_string(indent + 1) + "\n" + right_->to_string(indent + 1) +
           "\n" + op_to_string(op_);
  }

  int eval() override {
    if (op_ == Operator::PLUS) {
      return left_->eval() + right_->eval();
    } else if (op_ == Operator::TIMES) {
      return left_->eval() * right_->eval();
    } else if (op_ == Operator::LESS_THAN) {
      return left_->eval() < right_->eval();
    } else if (op_ == Operator::LOGICAL_AND) {
      return left_->eval() && right_->eval();
    } else if (op_ == Operator::LOGICAL_OR) {
      return left_->eval() || right_->eval();
    } else if (op_ == Operator::EQUALS) {
      return left_->eval() == right_->eval();
    }
    return 0;
  }

 private:
  std::unique_ptr<Expression> left_;
  std::unique_ptr<Expression> right_;
  Operator op_;
};

class Ast {
 public:
  Ast(std::unique_ptr<Expression> root) : root_(std::move(root)) {}
  int eval() { return root_->eval(); }
  std::unique_ptr<Expression>& root() { return root_; }

 private:
  std::unique_ptr<Expression> root_;
};

}  // namespace simp