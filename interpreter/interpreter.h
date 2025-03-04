#pragma once

#include "ast/ast.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "tokens/tokens.h"

namespace simp {
class Interpreter {
 public:
  Interpreter(const std::string& source) : source_(source) {
    lexer_ = Lexer(source);
    lexer_.scan();
    parser_ = Parser(lexer_.tokens());
    parser_.parse();
    ast_ = std::move(parser_.ast());
  }
  const std::string& source() const { return source_; }
  Lexer& lexer() { return lexer_; }
  Parser& parser() { return parser_; }
  Ast& ast() { return ast_; }
  bool run();

 private:
  Lexer lexer_;
  Parser parser_;
  Ast ast_;
  const std::string source_;
}
}  // namespace simp