#include "writer/xml/xml_writer.h"

namespace jack_compiler {
std::string XMLWriter::Write(const std::shared_ptr<Node>& root, int level) {
    std::string end_label(WriteEndLabel(root, level));

    if (root->IsTerminalToken()) {
        std::string content = root->GetContent();
        if (root->GetTokenType().terminal_token_ == TerminalTokenType::kSymbol) {
            if (content == ">") {
                content = "&gt;";
            } else if (content == "<") {
                content = "&lt;";
            } else if (content == "&") {
                content = "&amp;";
            }
        }
        return WriteStartLabel(root, level) + " " + content + " " + WriteEndLabel(root);
    }
    std::string res;
    res += WriteStartLabel(root, level);
    res.push_back('\n');
    for (auto& child : root->GetChildren()) {
        res += Write(child, level+1);
    }
    res += WriteEndLabel(root, level);
    return res;
}
}  // namespace jack_compiler
