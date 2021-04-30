#include "writer/vm/vm_writer.h"

namespace jack_compiler {
std::string VMWriter::Write(const std::shared_ptr<Node>& root, int level) {
    return root->GetContent() + std::to_string(level);
}

void WritePush() {}
void WritePop() {}
void WriteArithmetic() {}
void WriteLabel() {}
void WriteGoto() {}
void WriteIf() {}
void WriteCall() {}
void WriteFunction() {}
void WriteReturn() {}
void Close() {}

}  // namespace jack_compiler
