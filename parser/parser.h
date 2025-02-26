#include <memory>
#include <queue>

#include "ast/ast.h"
#include "tokens/tokens.h"
namespace simp {
class Parser {
 public:
  Parser(std::queue<std::unique_ptr<Token>> tokens)
      : tokens_(std::move(tokens)) {}
  std::unique_ptr<KeywordToken> expect_keyword(const std::string& keyword);
  std::unique_ptr<OperatorToken> expect_operator();
  bool parse();
  std::unique_ptr<Expression> parse_primary_expression();
  std::unique_ptr<Expression> parse_binary_expression();
  // TODO: check if move needed.
  std::unique_ptr<Ast> ast() { return std::move(ast_); }

 private:
  std::queue<std::unique_ptr<Token>> tokens_;
  std::unique_ptr<Ast> ast_;
};
}  // namespace simp