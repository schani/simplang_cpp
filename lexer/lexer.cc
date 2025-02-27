#include "lexer.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
// #define DEBUG

namespace simp {
using ::std::ifstream;
using ::std::string;

void handle_bad_file(std::ifstream& file) {
#ifdef DEBUG
  std::cout << "*******************Unable to open file************\n";
#endif
  if (file.bad()) {
    std::cerr << "Fatal error: badbit is set." << std::endl;
  } else if (file.fail()) {
    // Print a more detailed error message using
    // strerror
    std::cerr << "Error details: " << strerror(errno) << std::endl;
  }
}

bool Lexer::scan() {
#ifdef DEBUG
  std::cout << "Scanning file: " << file_name() << std::endl;
#endif
  std::ifstream f(file_name());
  if (!f.is_open()) {
    handle_bad_file(f);
    return false;
  }
  char c;
  std::string token = "";
  int line = 1;
  int position = 1;
  while (f.get(c)) {
#ifdef DEBUG
    std::cout << "--while(c=->" << c << "<-)" << std::endl;
#endif
    if (c == '(') {
      tokens_.push_back(std::make_unique<OperatorToken>(
          Operator::OPEN_PAREN, line, position, file_name()));
      position++;
      continue;
    } else if (c == ')') {
      tokens_.push_back(std::make_unique<OperatorToken>(
          Operator::CLOSE_PAREN, line, position, file_name()));
      position++;
      continue;
    } else if (c == '+') {
      tokens_.push_back(std::make_unique<OperatorToken>(Operator::PLUS, line,
                                                        position, file_name()));
      position++;
      continue;
    } else if (c == '*') {
      tokens_.push_back(std::make_unique<OperatorToken>(Operator::TIMES, line,
                                                        position, file_name()));
      position++;
      continue;
    } else if (c == '!') {
      tokens_.push_back(std::make_unique<OperatorToken>(Operator::NOT, line,
                                                        position, file_name()));
      position++;
      continue;
    } else if (c == '<') {
      tokens_.push_back(std::make_unique<OperatorToken>(
          Operator::LESS_THAN, line, position, file_name()));
      position++;
      continue;
    } else if (c == '-') {
      tokens_.push_back(std::make_unique<OperatorToken>(
          Operator::UNARY_MINUS, line, position, file_name()));
      position++;
      continue;
    } else if (c == '|') {
      if (f.peek() == '|') {
        f.get(c);
        position++;
        tokens_.push_back(std::make_unique<OperatorToken>(
            Operator::LOGICAL_OR, line, position, file_name()));
        continue;
      } else {
        std::cerr << "Error: expected || at line:" << line
                  << " at position:" << position << std::endl;
        return false;
      }
      continue;
    } else if (c == '&') {
      if (f.peek() == '&') {
        f.get(c);
        position++;
        tokens_.push_back(std::make_unique<OperatorToken>(
            Operator::LOGICAL_AND, line, position, file_name()));
        continue;
      } else {
        std::cerr << "Error: expected && at line:" << line
                  << " at position:" << position << std::endl;
        return false;
      }
      continue;
    } else if (c == '=') {
      if (f.peek() == '=') {
        f.get(c);
        position++;
        tokens_.push_back(std::make_unique<OperatorToken>(
            Operator::EQUALS, line, position, file_name()));
      } else {
        tokens_.push_back(std::make_unique<OperatorToken>(
            Operator::ASSIGN, line, position, file_name()));
      }
      continue;
    } else if (std::isdigit(c)) {
      token += c;
      position++;
      while (f.get(c) && std::isdigit(c)) {
        token += c;
        position++;
      }
      if (!std::isdigit(c)) {
        f.unget();
        position--;
        tokens_.push_back(std::make_unique<IntegerToken>(
            std::stoi(token), line, position - token.length(), file_name()));
        token = "";
        continue;
      }
    } else if (std::isspace(c)) {
#ifdef DEBUG
      std::cout << "----isspace" << std::endl;
#endif
      // ignore whitespace
      if (c == '\n') {
        line++;
        position = 1;
      } else {
        position++;
      }
    } else if (c == '_') {
      if (!std::isalpha(f.peek())) {
        std::cerr << "Error: expected identifier at line:" << line
                  << " at position:" << position << std::endl;
        return false;
      } else {
        token += c;
        while (f.get(c) && (std::isalnum(c) || c == '_')) {
          token += c;
        }
        if (std::isalnum(c) && c != '_') {
          f.unget();
          position--;
          tokens_.push_back(std::make_unique<IdentifierToken>(
              token, line, position - token.length(), file_name()));
          token = "";
          continue;
        }
      }
    } else if (std::isalpha(c)) {
      token += c;
      while (f.get(c) && (std::isalnum(c) || c == '_')) {
        token += c;
      }
      if (std::isspace(c) || f.peek() == EOF) {
        f.unget();
        position--;
        if (is_valid_keyword(token)) {
          tokens_.push_back(std::make_unique<KeywordToken>(
              token, line, position - token.length(), file_name()));
        } else {
          tokens_.push_back(std::make_unique<IdentifierToken>(
              token, line, position - token.length(), file_name()));
        }
        token = "";
        continue;
      }
      if (!std::isalnum(c) && c != '_') {
        f.unget();
        position--;
        if (is_valid_keyword(token)) {
          tokens_.push_back(std::make_unique<KeywordToken>(
              token, line, position - token.length(), file_name()));
        } else {
          tokens_.push_back(std::make_unique<IdentifierToken>(
              token, line, position - token.length(), file_name()));
        }
        token = "";
        continue;
      }
    }
  }
  return true;
}

}  // namespace simp