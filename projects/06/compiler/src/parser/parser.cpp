#include <algorithm>
#include <vector>

#include "parser.hpp"

namespace compiler
{
    Parser::Parser(std::istream* input): input_(input), code_(new Code), can_delete_input_(false) {
    };

    Parser::Parser(const std::string file): code_(new Code), can_delete_input_(true) {
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
        delete code_;
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

        if (need_valid_str[0] == '@') {
            // for A command
            std::string need_valid_symbol_or_int(need_valid_str, 1, need_valid_str.length() - 1);
            if (!IsValidSymbol(need_valid_symbol_or_int)) {
                try {
                    std::stol(need_valid_symbol_or_int);
                } catch(std::invalid_argument) {
                    return false;
                }
            }
            command_type_ = COMMAND::A_COMMAND;
            symbol_ = need_valid_symbol_or_int;
            return true;
        } else if (need_valid_str[0] == '(') {
            // for L command
            if (need_valid_str[need_valid_str.length() - 1] != ')') {
                return false;
            }

            std::string need_valid_symbol(need_valid_str, 1, need_valid_str.length() - 2);
            if (IsValidSymbol(need_valid_symbol)) {
                command_type_ = COMMAND::L_COMMAND;
                symbol_ = need_valid_symbol;
                return true;
            }
            return false;
        } else {
            // for C command
            command_type_ = COMMAND::C_COMMAND;
            
            // dest could be null
            std::size_t first_sep_pos = need_valid_str.find_first_of('=');
            if (first_sep_pos == std::string::npos) {
                dest_ = "null";
                first_sep_pos = -1;
            } else {
                dest_ = std::string(need_valid_str, 0, first_sep_pos);
            }
            if (!code_->IsValidDestCode(dest_)) {
                return false;
            }

            std::size_t second_sep_pos = need_valid_str.find_first_of(';');
            if (second_sep_pos == std::string::npos) {
                second_sep_pos = need_valid_str.length();
            }
            comp_ = std::string(need_valid_str, first_sep_pos + 1, second_sep_pos - (first_sep_pos + 1));
            if (!code_->IsValidCompCode(comp_)) {
                return false;
            }

            // jump could be null
            if (second_sep_pos == need_valid_str.length()) {
                jump_ = "null";
            } else {
                jump_ = std::string(need_valid_str, second_sep_pos + 1, need_valid_str.length());
            }
            if (!code_->IsValidJumpCode(jump_)) {
                return false;
            }

            return true;
        }
    }

    bool Parser::IsValidSymbol(const std::string symbol) {
        if (symbol[0] >= '0' && symbol[0] <= '9') {
            return false;
        }
        static std::vector<char> check_array = [] () {
            std::vector<char> res({'.', '_', '$', ':'});
            for (size_t i = 0; i < 26; ++i) {
                res.push_back('a' + i);
            }

            for (size_t i = 0; i < 26; ++i) {
                res.push_back('A' + i);
            }
            
            for (size_t i = 0; i < 10; ++i) {
                res.push_back('0' + i);
            }
            std::sort(res.begin(), res.end());
            return res;
        } ();

        for (size_t i = 0; i < symbol.length(); ++i) {
            if (!std::binary_search(check_array.begin(), check_array.end(), symbol[i])) {
                return false;
            }
        }
        return true;
    }
} // namespace compiler
