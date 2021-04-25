#include <fstream>
#include <iostream>

#include "compilation_engine/compilation_engine.hpp"
#include "jack_analyzer/jack_analyzer.hpp"
#include "jack_tokenizer/jack_tokenizer.hpp"
#include "utils/utils.hpp"

namespace jack_compiler {
    JackAnalyzer::JackAnalyzer(const std::string input_file_or_dir): file_or_dir_name_(input_file_or_dir) {
        ReadInput();
    }

    void JackAnalyzer::ReadInput() {
        // read all files
        Utils::ReadDir(file_or_dir_name_, files_);
        if (files_.empty()) {
            throw std::invalid_argument("Empty file or dir");
        }
    }

    void JackAnalyzer::Analyze(bool to_token) {
        for (const auto& file : files_) {
            size_t pos = file.find_last_of('.');
            std::string base_name(file, 0, pos);
            if (to_token) {
                const std::string output_file_name = base_name + "_token_analyze.xml";
                std::ofstream output(output_file_name);

                JackTokenizer token(file);
                XMLWriter writer(output_file_name);
                token.CreateOutput(writer);
            } else {
                const std::string output_file_name = base_name + "_analyze.xml";
                std::ofstream output(output_file_name);

                CompilationEngine engine(
                    file,
                    output_file_name
                );
                engine.CompileClass();
            }
        }
    }
} // namespace jack_compiler
