#include "writer/writer.hpp"


namespace jack_compiler {
    class XMLWriter: public Writer {
    public:
        explicit XMLWriter(const std::string& file): Writer(file) {};
        explicit XMLWriter(std::ostream* output): Writer(output) {};
        virtual ~XMLWriter() = default;
    private:
        inline std::string WriteStartLabel(std::shared_ptr<Node> node, int level = 0) {
            std::string content;
            for (int i = 0; i < level; ++i) {
                content.push_back('\t');
            }

            content += "<";
            if (node->IsTerminalToken()) {
                content += TERMINAL_TOKEN_TABLE.at(node->GetTokenType().terminal_token_);
            } else {
                content += NON_TERMINAL_TOKEN_TABLE.at(node->GetTokenType().non_terminal_token_);
            }
            return content + ">";
        };

        inline std::string WriteEndLabel(std::shared_ptr<Node> node, int level = 0) {
            std::string content;
            for (int i = 0; i < level; ++i) {
                content.push_back('\t');
            }

            content += "</";
            if (node->IsTerminalToken()) {
                content += TERMINAL_TOKEN_TABLE.at(node->GetTokenType().terminal_token_);
            } else {
                content += NON_TERMINAL_TOKEN_TABLE.at(node->GetTokenType().non_terminal_token_);
            }
            return content + ">\n";
        };
        virtual std::string Write(std::shared_ptr<Node> root, int level = 0);
    };

} // namespace jack_compiler
