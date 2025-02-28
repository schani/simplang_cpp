#include <deque>
#include <memory>

#include "ast/ast.h"
#include "tokens/tokens.h"
namespace simp {
class Parser {
 public:
  Parser(std::deque<std::unique_ptr<Token>> tokens)
      : tokens_(std::move(tokens)) {}
  std::unique_ptr<KeywordToken> expect_keyword(const std::string& keyword);
  std::unique_ptr<OperatorToken> expect_binary_operator();
  std::unique_ptr<OperatorToken> expect_close_paren();
  bool parse();
  std::unique_ptr<Expression> parse_primary_expression();
  std::unique_ptr<Expression> parse_binary_expression();
  void print_tokens() {
    while (!tokens_.empty()) {
      tokens_.front()->print();
      tokens_.pop_front();
    }
  }
  // TODO: check if move needed.
  std::unique_ptr<Ast> ast() { return std::move(ast_); }

 private:
  std::deque<std::unique_ptr<Token>> tokens_;
  std::unique_ptr<Ast> ast_;
};
}  // namespace simp