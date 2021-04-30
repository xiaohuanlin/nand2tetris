#ifndef JACK_COMPILER_NODE_H_
#define JACK_COMPILER_NODE_H_

#include <memory>
#include <string>
#include <vector>

#include "utils/enum/enum.h"

namespace jack_compiler {
/**
 * This class is used for building tree
 */
class Node {
 public:
  union TokenUnion {
    TERMINAL_TOKEN_TYPE terminal_token_;
    NON_TERMINAL_TOKEN_TYPE non_terminal_token_;
  };
  Node(bool is_terminal_token, TokenUnion token_type,
       const std::string &content = "");
  Node(const Node &) = delete;
  Node(Node &&) = delete;
  ~Node() = default;
  /**
   * Add child node for this node
   */
  void AppendChild(std::shared_ptr<Node> child);
  inline std::vector<std::shared_ptr<Node>> &GetChildren() noexcept {
    return children_;
  };
  inline TokenUnion GetTokenType() const noexcept { return token_type_; };
  /**
   * For terminal token, the content is used for showing the detail of this node
   */
  inline std::string GetContent() const noexcept { return content_; };
  inline bool IsTerminalToken() const noexcept { return is_terminal_token_; };

 private:
  bool is_terminal_token_;
  TokenUnion token_type_;
  std::string content_;
  std::vector<std::shared_ptr<Node>> children_;
};
}  // namespace jack_compiler

#endif  // JACK_COMPILER_NODE_H_
