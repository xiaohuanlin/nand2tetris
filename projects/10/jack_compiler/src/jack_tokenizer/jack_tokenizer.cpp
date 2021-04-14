#include "jack_tokenizer/jack_tokenizer.hpp"
#include "utils/enum/enum.hpp"

namespace jack_compiler {
    JackTokenizer::JackTokenizer(std::istream* input): input_(input) {
    }
    
    bool JackTokenizer::HasMoreTokens() {
        if (input_->eof()) {
            return false;
        }
        if (input_->peek() == EOF) {
            return false;
        }
        return true;
    }

    void JackTokenizer::Advance() {
        char buffer[BUFFER_SIZE];
        do {
            input_->getline(buffer, BUFFER_SIZE);
        } while (!IsValidToken(buffer) && !input_->eof());
    }

    bool JackTokenizer::IsValidToken(std::string need_valid_str) {
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
        return true;
    }
} // namespace jack_compiler
