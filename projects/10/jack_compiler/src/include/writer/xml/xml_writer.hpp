#include "writer/writer.hpp"


namespace jack_compiler {
    class XMLWriter: public Writer {
    public:
        explicit XMLWriter(const std::string& file): Writer(file) {};
        explicit XMLWriter(std::ostream* output): Writer(output) {};
        virtual ~XMLWriter() = default;
    private:
        inline std::string WriteStartLabel(std::shared_ptr<Node> node) {
            if (node->IsTerminalToken()) {
                return "<" + TERMINAL_TOKEN_TABLE.at(node->GetTokenType().terminal_token_) + ">";
            } else {
                return "<" + NON_TERMINAL_TOKEN_TABLE.at(node->GetTokenType().non_terminal_token_) + ">";
            }
        };

        inline std::string WriteEndLabel(std::shared_ptr<Node> node) {
            if (node->IsTerminalToken()) {
                return "</" + TERMINAL_TOKEN_TABLE.at(node->GetTokenType().terminal_token_) + ">";
            } else {
                return "</" + NON_TERMINAL_TOKEN_TABLE.at(node->GetTokenType().non_terminal_token_) + ">";
            }
        };
        virtual std::string Write(std::shared_ptr<Node> root);
    };

} // namespace jack_compiler
