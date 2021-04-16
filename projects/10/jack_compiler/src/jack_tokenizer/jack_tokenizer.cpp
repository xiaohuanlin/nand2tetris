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
        char buffer;
        do {
            input_->get(buffer);
        } while (!input_->eof() && !IsValidToken(buffer));
    }

    bool JackTokenizer::IsValidToken(char c) {
        if (c == ' ' || c == '\r' || c == '\n') {
            return false;
        }

        if (IsValidComment(c)) {
            return true;
        } else if (IsValidSymbol(c)) {
            return true;
        } else if (IsValidInteger(c)) {
            return true;
        } else if (IsValidString(c)) {
            return true;
//        } else {
//            char token_buffer[BUFFER_SIZE];
//            int i = 0;
//            do {
//                token_buffer[i++] = c;
//                input_->get(c);
//            } while (!input_->eof() && !IsValidToken(buffer));
        }
        return false;
    }

    bool JackTokenizer::IsValidComment(char c) {
        if (c == '/') {
            char next_word = char(input_->peek());
            if (next_word == '/') {
                // remove line comment
                do {
                    input_->get(c);
                } while (!input_->eof() && c != '\n');
                return true;
            } else if (next_word == '*') {
                // remove normal or document comment
                do {
                    input_->get(c);
                } while (!input_->eof() && !(c == '*' && input_->peek() == '/'));
                if (!input_->eof()) {
                    // eat the slash signal
                    input_->get(c);
                }
                return true;
            }
        }
        return false;
    }

    bool JackTokenizer::IsValidSymbol(char c) {
        if (SYMBOL_TABLE.find(c) != SYMBOL_TABLE.end()) {
            token_type_ = TOKEN_TYPE::SYMBOL;
            symbol_ = c;
            return true;
        }
        return false;
    }

    bool JackTokenizer::IsValidInteger(char c) {
        if (!(c >= '0' && c <= '9')) {
            return false;
        }
        std::string integer_string(1, c);
        while (!input_->eof() && (input_->peek() >= '0' && input_->peek() <= '9')) {
            input_->get(c);
            integer_string.push_back(c);
        }

        token_type_ = TOKEN_TYPE::INT_CONST;
        intval_ = std::stoi(integer_string);
        return true;
    }

    bool JackTokenizer::IsValidString(char c) {
        if (c != '"') {
            return false;
        }

        std::string str;
        while (!input_->eof()) {
            input_->get(c);
            if (c == '\n' || c == '"') {
                break;
            }
            str.push_back(c);
        }

        token_type_ = TOKEN_TYPE::STRING_CONST;
        strval_ = str;
        return true;
    }

    bool JackTokenizer::IsValidIdentifier(char c) {
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_')) {
            return false;
        }

        std::string str(1, c);
        while (!input_->eof()) {
            auto peek = input_->peek();
            if (!((peek >= 'A' && peek <= 'Z') || (peek >= 'a' && peek <= 'z') || peek == '_' || (peek >= '0' && peek <= '9'))) {
                break;
            }
            input_->get(c);
            str.push_back(c);
        }

        if (IsValidKeyWord(str)) {
            return true;
        }

        token_type_ = TOKEN_TYPE::IDENTIFIER;
        identifier_ = str;
        return true;
    }

    bool JackTokenizer::IsValidKeyWord(const std::string &keyword) {
        auto iter = KEYWORD_TABLE.find(keyword);
        if (iter != KEYWORD_TABLE.end()) {
            token_type_ = TOKEN_TYPE::KEYWORD;
            key_word_ = iter->second;
            return true;
        }
        return false;
    }

} // namespace jack_compiler
