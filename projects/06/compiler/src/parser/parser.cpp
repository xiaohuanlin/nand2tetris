#include "parser.hpp"

namespace compiler
{
    Parser::Parser(std::istream* input): input_(input) {};

    Parser::Parser(const std::string file) {
        std::ifstream* open_file_p = new std::ifstream(file);
        if (!open_file_p->is_open()) {
            throw std::exception();
        }
        input_ = open_file_p;
    }

    bool Parser::HasMoreCommands() {
        return input_->eof();
    }

    void Parser::Advance() {

    }
} // namespace compiler
