#include <algorithm>
#include <vector>

#include "parser.hpp"

namespace vmtranslator
{
    Parser::Parser(std::istream* input): input_(input), can_delete_input_(false) {
    };

    Parser::Parser(const std::string file): can_delete_input_(true) {
        std::ifstream* open_file_p = new std::ifstream(file);
        if (!open_file_p->is_open()) {
            delete open_file_p;
            throw std::exception();
        }
        input_ = open_file_p;
    }
    
    Parser::~Parser(){
        if (can_delete_input_) {
            delete reinterpret_cast<std::ifstream*>(input_);
        }
    }

    bool Parser::HasMoreCommands() {
        if (input_->eof()) {
            return false;
        }
        if (input_->peek() == EOF) {
            return false;
        }
        return true;
    }

    void Parser::Advance() {
        do {
            input_->getline(current_command_, BUFFER_SIZE);
        } while (!IsValidCommand() && !input_->eof());
    }

    bool Parser::IsValidCommand() {
        std::string need_valid_str(current_command_);
        // remove \r in windows system
        if (need_valid_str[need_valid_str.length() - 1] == '\r') {
            need_valid_str.erase(need_valid_str.length() - 1);
        }

        // remove comment
        size_t first_slash = need_valid_str.find_first_of("/");
        if (first_slash + 1 < need_valid_str.length() && need_valid_str[first_slash+1] == '/') {
            need_valid_str.erase(first_slash);
        }

        // remove spaces at head
        need_valid_str.erase(0, need_valid_str.find_first_not_of(" "));
        // remove spaces at tail
        need_valid_str.erase(need_valid_str.find_last_not_of(" ") + 1);

        if (need_valid_str.empty() || (need_valid_str[0] == '/' && need_valid_str[1] == '/')) {
            // empty string or comment
            return false;
        }

        if (IsValidArithmeticCommand(need_valid_str)) {
            if (!ParseCommand(need_valid_str)) {
                return false;
            }
            arg1_ = need_valid_str;
            return true;
        } else if (IsValidMemoryAccessCommand(need_valid_str)) {
            if (!ParseCommand(need_valid_str)) {
                return false;
            }
            return true;
        }

        return false;
    }

    bool Parser::ParseCommand(const std::string& command) {
        size_t len = command.length();
        size_t pos = 0;
        int index = 0;

        for (size_t i = 0; i <= len;) {
            if (i == len || command[i] == ' ') {
                if (index == 0) {
                    // for command_type
                    auto iter = commands_form_.find(std::string(command, pos, i - pos));
                    if (iter == commands_form_.end()) {
                        return false;
                    }
                    command_type_ = iter->second;
                } else if (index == 1) {
                    // for arg1
                    arg1_ = std::string(command, pos, i - pos);
                } else if (index == 2){
                    // for arg2
                    try {
                        arg2_ = std::stod(std::string(command, pos, i - pos));
                    } catch(std::invalid_argument) {
                        return false;
                    }
                } else {
                    return false;
                }
                index++;
                do {
                    i++;
                } while (i < len && command[i] == ' ');
                pos = i;
            } else {
                i++;
            }
        }
        return true;
    }

    bool Parser::IsValidArithmeticCommand(const std::string& command) {
        return arithmetic_commands_.find(command) != arithmetic_commands_.end();
    }

    bool Parser::IsValidMemoryAccessCommand(const std::string& command) {
        size_t pos = command.find_first_of(" ");
        if (pos == std::string::npos) {
            return false;
        }
        std::string command_type = std::string(command, 0, pos);
        return command_type == "push" || command_type == "pop";
    }

} // namespace vmtranslator
