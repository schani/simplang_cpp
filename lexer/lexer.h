#pragma once

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
  const std::vector<std::string>& tokens() const { return tokens_; }
  const std::string& file_name() const { return file_name_; }

 private:
  const std::string file_name_;
  std::vector<std::string> tokens_;
};

}  // namespace simp