#include "jack_compiler/jack_compiler.h"

#include <string>
#include <iostream>

#include "compilation_engine/compilation_engine.h"
#include "jack_tokenizer/jack_tokenizer.h"
#include "utils/utils.h"
#include "writer/vm/vm_writer.h"
#include "writer/xml/xml_writer.h"

namespace jack_compiler {

JackCompiler::JackCompiler(const std::string& input_file_or_dir): file_or_dir_name_(input_file_or_dir) {
    ReadInput();
}

void JackCompiler::ReadInput() {
    // read all files
    utils::ReadDir(file_or_dir_name_, &files_);
    if (files_.empty()) {
        throw std::invalid_argument("Empty file or dir");
    }
}

void JackCompiler::Analyze(std::string writer_class) {
    for (const auto& file : files_) {
        size_t pos = file.find_last_of('.');
        std::string base_name(file, 0, pos);
        const std::string output_file_name = base_name + ".vm";
        std::ofstream output(output_file_name);

        if (writer_class == "xml_writer") {
            CompilationEngine<XMLWriter> engine(file, output_file_name);
            engine.CompileClass();
        } else if (writer_class == "vm_writer") {
            CompilationEngine<VMWriter> engine(file, output_file_name);
            engine.CompileClass();
        } else {
            exit(1);
        }
    }
}
}  // namespace jack_compiler
