
#include "writer/node.hpp"

namespace jack_compiler {
    Node::Node(bool is_terminal_token, TokenUnion token_type, const std::string& content): 
        is_terminal_token_(is_terminal_token), token_type_(token_type), content_(content) {};

    void Node::AppendChild(std::shared_ptr<Node> child) {
        children_.push_back(std::move(child));
    }
}