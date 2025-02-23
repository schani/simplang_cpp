#include "lexer.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

namespace simp {
using ::std::ifstream;
using ::std::string;

void handle_bad_file(std::ifstream& file) {
  std::cout << "*******************Unable to open file************\n";
  if (file.bad()) {
    std::cerr << "Fatal error: badbit is set." << std::endl;
  } else if (file.fail()) {
    // Print a more detailed error message using
    // strerror
    std::cerr << "Error details: " << strerror(errno) << std::endl;
  }
}

bool is_alpha(char c) { return std::isalpha(c) || c == '_'; }

bool Lexer::scan() {
  std::cout << "Scanning file: " << file_name() << std::endl;

  std::ifstream f(file_name());
  if (!f.is_open()) {
    handle_bad_file(f);
    return false;
  }
  char c;
  std::string token = "";
  State state = State::START;
  int line = 1;
  int position = 1;
  while (f.get(c)) {
    std::cout << "--while(c=->" << c << "<-)" << std::endl;
    switch (state) {
      case State::START: {
        std::cout << "----Start" << std::endl;
        if (c == '(') {
          tokens_.push_back(std::make_unique<OperatorToken>(
              Operator::OPEN_PAREN, line, position, file_name()));
          continue;
        } else if (c == ')') {
          tokens_.push_back(std::make_unique<OperatorToken>(
              Operator::CLOSE_PAREN, line, position, file_name()));
          continue;
        }
      }
      default:
        break;
    }
  }
  return true;
}

}  // namespace simp