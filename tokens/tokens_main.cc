#include "tokens.h"

int main() {
  std::vector<std::unique_ptr<simp::Token>> tokens;
  tokens.push_back(std::make_unique<simp::IntegerToken>(123, 1, 1, "file"));
  tokens.push_back(std::make_unique<simp::OperatorToken>(
      simp::Operator::UNARY_MINUS, 2, 3, "file"));
  tokens.push_back(std::make_unique<simp::KeywordToken>("let", 4, 5, "file"));
  tokens.push_back(std::make_unique<simp::IdentifierToken>("x", 6, 7, "file"));
  for (auto& token : tokens) {
    token->print();
  }

  return 0;
}