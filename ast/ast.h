#pragma once
#include <iostream>
#include <memory>

#include "tokens/tokens.h"

namespace simp {
enum class ExpressionType {
  INTEGER,
  IF,
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
  virtual void print() = 0;
  virtual std::string to_string() = 0;
  ExpressionType type() { return type_; }

 private:
  ExpressionType type_;
};

class IntExpression : public Expression {
 public:
  IntExpression(int value)
      : Expression(ExpressionType::INTEGER), value_(value) {}
  int eval() override { return value_; }
  void print() override {
    std::cout << "Int expression with value:" << value_ << std::endl;
  }
  std::string to_string() override { return std::to_string(value_); }

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
               std::unique_ptr<Expression> alternative)
      : Expression(ExpressionType::IF),
        if_token_(std::move(if_token)),
        condition_(std::move(condition)),
        then_token_(std::move(then_token)),
        consequent_(std::move(consequent)),
        else_token_(std::move(else_token)),
        alternative_(std::move(alternative)) {}

  std::unique_ptr<KeywordToken>& if_token() { return if_token_; }
  std::unique_ptr<KeywordToken>& then_token() { return then_token_; }
  std::unique_ptr<KeywordToken>& else_token() { return else_token_; }
  std::unique_ptr<Expression>& condition() { return condition_; }
  std::unique_ptr<Expression>& consequent() { return consequent_; }
  std::unique_ptr<Expression>& alternative() { return alternative_; }

  int eval() override {
    if (condition_->eval()) {
      return consequent_->eval();
    }
    return alternative_->eval();
  }
  void print() override { std::cout << to_string() << std::endl; }
  std::string to_string() override {
    return "If\ncondition:" + condition_->to_string() +
           "\nthen:" + consequent_->to_string() +
           "\nelse:" + alternative_->to_string();
  }

 private:
  std::unique_ptr<KeywordToken> if_token_;
  std::unique_ptr<KeywordToken> then_token_;
  std::unique_ptr<KeywordToken> else_token_;
  std::unique_ptr<Expression> condition_;
  std::unique_ptr<Expression> consequent_;
  std::unique_ptr<Expression> alternative_;
};

}  // namespace simp