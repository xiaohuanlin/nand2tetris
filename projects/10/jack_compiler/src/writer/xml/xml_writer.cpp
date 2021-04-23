#include "writer/xml/xml_writer.hpp"

namespace jack_compiler {
    std::string XMLWriter::Write(std::shared_ptr<Node> root, int level) {
        std::string end_label(WriteEndLabel(root, level));

        if (root->IsTerminalToken()) {
            return WriteStartLabel(root, level) + " " + root->GetContent() + " " + WriteEndLabel(root);
        }
        std::string res;
        res += WriteStartLabel(root, level);
        res.push_back('\n');
        for (auto& child : root->GetChildren()) {
            res += Write(std::move(child), level+1);
        }
        res += WriteEndLabel(root, level);
        return res;
    }
} // namespace jack_compiler