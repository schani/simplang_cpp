#pragma once

#include <memory>
#include <string>
#include <vector>

#include "tokens.h"

namespace simp {
enum State {
  START,
  IN_INTEGER,
  IN_IDENTIFIER,
  IN_KEYWORD,
};
class Lexer {
 public:
  Lexer(const std::string& file_name) : file_name_(file_name) {}

  bool scan();
  std::vector<std::unique_ptr<Token>>& tokens() { return tokens_; }
  const std::string& file_name() const { return file_name_; }

 private:
  const std::string file_name_;
  std::vector<std::unique_ptr<Token>> tokens_;
};

}  // namespace simp