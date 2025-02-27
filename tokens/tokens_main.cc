#include <queue>

#include "tokens.h"
// #include <string>
#include <iostream>

// class Lexer {
//  public:
//   Lexer(const std::string& file_name) : file_name_(file_name) {}

//   bool scan();
//   std::queue<std::unique_ptr<simp::Token>>& tokens() { return tokens_; }
//   const std::string& file_name() const { return file_name_; }

//  private:
//   const std::string file_name_;
//   std::queue<std::unique_ptr<simp::Token>> tokens_;
// };

int main() {
  std::queue<std::unique_ptr<simp::Token>> tokens;
  tokens.push(std::make_unique<simp::IntegerToken>(123, 1, 1, "file"));
  tokens.push(std::make_unique<simp::OperatorToken>(simp::Operator::UNARY_MINUS,
                                                    2, 3, "file"));
  tokens.push(std::make_unique<simp::KeywordToken>("let", 4, 5, "file"));
  while (!tokens.empty()) {
    // auto token = std::move(tokens.front());
    tokens.front()->print();
    tokens.pop();
  }

  return 0;
}