#include "lexer.h"
#undef GOOGLE_STRIP_LOG
#define GOOGLE_STRIP_LOG 1
#include <glog/logging.h>

#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>

namespace simp {
using ::std::ifstream;
using ::std::string;

void handle_bad_file(std::ifstream& file) {
  LOG(INFO) << "*******************Unable to open file************\n";
  if (file.bad()) {
    LOG(ERROR) << "Fatal error: badbit is set.";
  } else if (file.fail()) {
    // Print a more detailed error message using
    // strerror
    LOG(ERROR) << "Error details: " << strerror(errno);
  }
}

bool Lexer::scan() {
  LOG(INFO) << "Scanning file: " << file_name();
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
    LOG(INFO) << "--while(c=->" << c << "<-)";
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
        LOG(ERROR) << "Expected || at line:" << line
                   << " at position:" << position << " but only found one |";
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
        LOG(ERROR) << "Expected && at line:" << line
                   << " at position:" << position << " but only found one &";
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
      if (!std::isdigit(c) || f.peek() == EOF) {
        f.unget();
        position--;
        tokens_.push_back(std::make_unique<IntegerToken>(
            std::stoi(token), line, position - token.length(), file_name()));
        token = "";
        continue;
      }
    } else if (std::isspace(c)) {
      LOG(INFO) << "----isspace" << std::endl;
      // ignore whitespace unless it's a newline
      if (c == '\n') {
        line++;
        position = 1;
      } else {
        position++;
      }
    } else if (c == '_') {
      if (!std::isalpha(f.peek())) {
        LOG(ERROR) << "Expected identifier at line:" << line
                   << " at position:" << position
                   << " but found non alpha char after _ in the front";
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