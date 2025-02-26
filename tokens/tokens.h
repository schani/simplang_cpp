#pragma once

#include <iostream>
#include <map>
#include <string>

namespace simp {

enum TokenType {
  KEYWORD,
  INTEGER,
  IDENTIFIER,
  OPERATOR,
};

inline std::map<std::string, std::string> Keywords = {
    {"let", "let-keyword"},     {"end", "end-keyword"},
    {"recur", "recur-keyword"}, {"if", "if-keyword"},
    {"then", "then-keyword"},   {"else", "else-keyword"},
    {"in", "in-keyword"},       {"and", "and-keyword"},
    {"loop", "loop-keyword"},
};

inline bool is_valid_keyword(std::string keyword) {
  return Keywords.find(keyword) != Keywords.end();
}

inline bool is_keyword_prefix(const std::string& prefix) {
  for (const auto& pair : Keywords) {
    if (pair.first.rfind(prefix, 0) == 0) {
      return true;
    }
  }
  return false;
}

inline std::string to_string(std::string keyword) {
  if (!is_valid_keyword(keyword)) {
    return "invalid-keyword";
  }
  return Keywords[keyword];
}

enum Operator {
  // single operators
  OPEN_PAREN,
  CLOSE_PAREN,
  PLUS,
  TIMES,
  NOT,
  LESS_THAN,
  UNARY_MINUS,
  ASSIGN,
  // doouble operators
  EQUALS,
  LOGICAL_OR,
  LOGICAL_AND,
};

inline std::string op_to_string(Operator op) {
  switch (op) {
    case LOGICAL_OR:
      return "logical-or-operator";
    case LOGICAL_AND:
      return "logical-and-operator";
    case NOT:
      return "not-operator";
    case LESS_THAN:
      return "less-than-operator";
    case EQUALS:
      return "equals-operator";
    case ASSIGN:
      return "assign-operator";
    case PLUS:
      return "plus-operator";
    case TIMES:
      return "times-operator";
    case UNARY_MINUS:
      return "unary-minus-operator";
    case OPEN_PAREN:
      return "open-paren-operator";
    case CLOSE_PAREN:
      return "close-paren-operator";
  }
}

inline std::string operators = "+-*(=)&|!";
inline bool is_operator(char c) {
  return operators.find(c) != std::string::npos;
}

class Token {
 public:
  Token(TokenType type, int line, int position, std::string file_name)
      : type_(type), line_(line), position_(position), file_name_(file_name) {}

  TokenType type() { return type_; }
  int line() { return line_; }
  int position() { return position_; }
  std::string file_name() { return file_name_; }
  std::string location() {
    return " in file:\"" + file_name_ + "\"\ton line:" + std::to_string(line_) +
           "\tat position:" + std::to_string(position_);
  }

  /**************** Virtual methods******************/
  virtual std::string to_string() = 0;
  virtual void print() = 0;
  virtual ~Token() {}

 private:
  TokenType type_;
  int line_;
  int position_;
  std::string file_name_;
};

class OperatorToken : public Token {
 public:
  OperatorToken(Operator op, int line, int position, std::string file_name)
      : Token(TokenType::OPERATOR, line, position, file_name), op_(op) {
    std::cout << "(" << op_to_string(op) << ")" << "OperatorToken created"
              << std::endl;
  }

  OperatorToken(OperatorToken* op_token)
      : Token(TokenType::OPERATOR, op_token->line(), op_token->position(),
              op_token->file_name()),
        op_(op_token->op()) {
    std::cout << "(" << op_to_string(op_) << ")" << "OperatorToken created"
              << std::endl;
  }

  Operator op() { return op_; }
  std::string to_string() override { return op_to_string(op_); }
  void print() override {
    std::cout << "OperatorToken(" << to_string() << ")" << location()
              << std::endl;
  }

  ~OperatorToken() {
    std::cout << "OperatorToken(" << to_string() << ") destroyed" << std::endl;
  }

 private:
  Operator op_;
};

class IntegerToken : public Token {
 public:
  IntegerToken(int value, int line, int position, std::string file_name)
      : Token(TokenType::INTEGER, line, position, file_name), value_(value) {
    std::cout << "IntegerToken created with value:" << value_ << std::endl;
  }
  int value() { return value_; }
  std::string to_string() override { return std::to_string(value_); }
  void print() override {
    std::cout << "IntegerToken(" << value_ << ")" << location() << std::endl;
  }

  ~IntegerToken() {
    std::cout << "IntegerToken(" << value_ << ") destroyed" << std::endl;
  }

 private:
  const int value_;
};

class KeywordToken : public Token {
 public:
  KeywordToken(std::string keyword, int line, int position,
               std::string file_name)
      : Token(TokenType::KEYWORD, line, position, file_name),
        keyword_(keyword) {
    if (!is_valid_keyword(keyword_)) {
      throw std::runtime_error("Invalid keyword");
    } else {
      std::cout << "(" << keyword << ") KeywordToken created" << std::endl;
    }
  }

  KeywordToken(KeywordToken* keyword_token)
      : Token(TokenType::KEYWORD, keyword_token->line(),
              keyword_token->position(), keyword_token->file_name()),
        keyword_(keyword_token->keyword()) {
    if (!is_valid_keyword(keyword_)) {
      throw std::runtime_error("Invalid keyword");
    } else {
      std::cout << "(" << keyword_ << ") KeywordToken created" << std::endl;
    }
  }

  std::string keyword() { return keyword_; }
  std::string to_string() override { return Keywords[keyword_]; }
  void print() override {
    std::cout << "KeywordToken(" << keyword_ << ")" << location() << std::endl;
  }
  ~KeywordToken() {
    std::cout << "KeywordToken(" << keyword_ << ") destroyed" << std::endl;
  }

 private:
  std::string keyword_;
};

class IdentifierToken : public Token {
 public:
  IdentifierToken(std::string name, int line, int position,
                  std::string file_name)
      : Token(TokenType::IDENTIFIER, line, position, file_name), name_(name) {
    std::cout << "IdentifierToken(" << name_ << ") created" << std::endl;
  }
  std::string name() { return name_; }
  std::string to_string() override { return name_; }
  void print() override {
    std::cout << "IdentifierToken(" << name_ << ")" << location() << std::endl;
  }

  ~IdentifierToken() {
    std::cout << "IdentifierToken(" << name_ << ") destroyed" << std::endl;
  }

 private:
  std::string name_;
};

}  // namespace simp