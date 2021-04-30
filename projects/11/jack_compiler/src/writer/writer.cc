#include <typeinfo>

#include "writer/writer.h"

namespace jack_compiler {
Writer::Writer(std::ostream* output): can_delete_output_(false), output_(output) {}

Writer::Writer(const std::string& file) {
    std::ofstream* open_file_p = new std::ofstream(file);
    if (!open_file_p->is_open()) {
        delete open_file_p;
        std::cerr << "Output file can't be opened" << std::endl;
        throw std::exception();
    }
    can_delete_output_ = true;
    output_ = open_file_p;
}

Writer::~Writer() {
    if (can_delete_output_) {
        delete reinterpret_cast<std::ofstream*>(output_);
    }
}

void Writer::WriteToOutput(const std::shared_ptr<Node>& root) {
    (*output_) << Write(root);
    output_->flush();
}
}  // namespace jack_compiler
