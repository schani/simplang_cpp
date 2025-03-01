#pragma once

#include <deque>
#include <memory>
#include <string>
#undef GOOGLE_STRIP_LOG
#define GOOGLE_STRIP_LOG 1
#include <glog/logging.h>

#include "tokens/tokens.h"

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
  std::deque<std::unique_ptr<Token>>& tokens() { return tokens_; }
  const std::string& file_name() const { return file_name_; }
  void print_tokens() {
    while (!tokens_.empty()) {
      std::cout << tokens_.front()->to_string();
      tokens_.pop_front();
    }
  }

 private:
  const std::string file_name_;
  std::deque<std::unique_ptr<Token>> tokens_;
};

}  // namespace simp