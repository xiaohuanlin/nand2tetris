#include "writer/xml/xml_writer.hpp"

namespace jack_compiler {
    std::string XMLWriter::Write(std::shared_ptr<Node> root) {
        std::string start_label(WriteStartLabel(root));
        std::string end_label(WriteEndLabel(root));

        if (root->IsTerminalToken()) {
            return start_label + root->GetContent() + end_label;
        }
        std::string res;
        res += start_label;
        for (auto& child : root->GetChildren()) {
            res += Write(std::move(child));
        }
        res += end_label;
        return res;
    }
} // namespace jack_compiler