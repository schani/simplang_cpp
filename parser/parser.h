#include <memory>

#include "ast/ast.h"
#include "tokens/tokens.h"
namespace simp {
class Parser {
 public:
  Parser(std::vector<std::unique_ptr<Token>> tokens);
  std::unique_ptr<Expression> parse(int start_index = 0);

 private:
  std::vector<std::unique_ptr<Token>> tokens_;
  std::unique_ptr<ast::AST> ast_;
};
}  // namespace simp