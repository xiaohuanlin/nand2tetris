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

    void JackAnalyzer::Analyze() {
        for (const auto& file : files_) {
            size_t pos = file.find_last_of('.');
            std::string base_name(file, 0, pos);
            const std::string output_file_name = base_name + ".xml";
            std::ofstream output(output_file_name);
            if (!output.is_open()) {
                std::cerr << "Can't open the output file: " << output_file_name << std::endl;
                continue;
            }

            std::ifstream input(file);
            if (!input.is_open()) {
                std::cerr << "Can't open the input file: " << file << std::endl;
                continue;
            }

            JackTokenizer tokenizer(reinterpret_cast<std::istream*>(&input));
            CompilationEngine engine(reinterpret_cast<std::istream*>(&input), reinterpret_cast<std::ostream*>(&output));

            while (tokenizer.HasMoreTokens()) {
                tokenizer.Advance();
                
                switch (tokenizer.GetTokenType()) {
                    case TOKEN_TYPE::KEYWORD: {
                        break;
                    }
                    case TOKEN_TYPE::SYMBOL: {
                        break;
                    }
                    case TOKEN_TYPE::IDENTIFIER: {
                        break;
                    }
                    case TOKEN_TYPE::INT_CONST: {
                        break;
                    }
                    case TOKEN_TYPE::STRING_CONST: {
                        break;
                    }
                    default:
                        std::cerr << "Unknown token" << std::endl;
                        break;
                }
            }
        }
    }
} // namespace jack_compiler
