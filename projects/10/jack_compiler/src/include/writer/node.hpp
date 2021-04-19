#pragma once

#include <memory>
#include <string>
#include <vector>

#include "utils/enum/enum.hpp"

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
        Node(bool is_terminal_token, TokenUnion token_type, const std::string& content = "");
        /**
         * Add child node for this node
         */
        void AppendChild(std::shared_ptr<Node> child);
        inline std::vector<std::shared_ptr<Node>>& GetChildren() {return children_;};
        inline TokenUnion GetTokenType() {return token_type_;};
        /**
         * For terminal token, the content is used for showing the detail of this node
         */
        inline std::string GetContent() {return content_;};
        inline bool IsTerminalToken() {return is_terminal_token_;};
    private:
        bool is_terminal_token_;
        TokenUnion token_type_;
        std::string content_;
        std::vector<std::shared_ptr<Node>> children_;
    };
} // namespace jack_compiler