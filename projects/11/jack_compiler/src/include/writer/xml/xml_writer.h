#ifndef JACK_COMPILER_XML_WRITER_H_
#define JACK_COMPILER_XML_WRITER_H_

#include <memory>
#include <string>

#include "writer/writer.h"

namespace jack_compiler {
class XMLWriter : public Writer {
 public:
  explicit XMLWriter(const std::string &file) : Writer(file) {}
  explicit XMLWriter(std::ostream *output) : Writer(output) {}
  XMLWriter(const XMLWriter &) = delete;
  XMLWriter(XMLWriter &&) = delete;
  virtual ~XMLWriter() = default;

 private:
  inline std::string WriteStartLabel(std::shared_ptr<Node> node,
                                     int level = 0) {
    std::string content;
    for (int i = 0; i < level; ++i) {
      content.push_back('\t');
    }

    content += "<";
    if (node->IsTerminalToken()) {
      content += TERMINAL_TOKEN_TABLE.at(node->GetTokenType().terminal_token_);
    } else {
      content +=
          NON_TERMINAL_TOKEN_TABLE.at(node->GetTokenType().non_terminal_token_);
    }
    return content + ">";
  }

  inline std::string WriteEndLabel(std::shared_ptr<Node> node, int level = 0) {
    std::string content;
    for (int i = 0; i < level; ++i) {
      content.push_back('\t');
    }

    content += "</";
    if (node->IsTerminalToken()) {
      content += TERMINAL_TOKEN_TABLE.at(node->GetTokenType().terminal_token_);
    } else {
      content +=
          NON_TERMINAL_TOKEN_TABLE.at(node->GetTokenType().non_terminal_token_);
    }
    return content + ">\n";
  }
  std::string Write(const std::shared_ptr<Node> &root, int level = 0) final;
};

}  // namespace jack_compiler

#endif  // JACK_COMPILER_XML_WRITER_H_
