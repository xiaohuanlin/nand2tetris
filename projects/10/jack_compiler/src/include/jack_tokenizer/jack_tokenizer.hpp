#pragma once

#include <iostream>
#include "utils/enum/enum.hpp"

namespace jack_compiler {
    class JackTokenizer {
    public:
        JackTokenizer(std::istream* input);
        JackTokenizer(const std::string file);
        ~JackTokenizer();

        /**
         * If there are more tokens?
         */
        bool HasMoreTokens();

        /**
         * Only if there are more tokens, we can use this function to next token
         */
        void Advance();

        inline const TOKEN_TYPE GetTokenType() const noexcept {return token_type_;};
        inline const KEYWORD_TYPE GetKeyword() const noexcept {return parse_value_.key_word_;};
        inline const std::string GetSymbol() const noexcept {return parse_value_.symbol_;};
        inline const std::string GetIdentifier() const noexcept {return parse_value_.iden_;};
        inline const int GetIntVal() const noexcept {return parse_value_.intval_;};
        inline const std::string GetStringVal() const noexcept {return parse_value_.strval_;};
    private:
        const TOKEN_TYPE token_type_;
        union ParseValue {
            KEYWORD_TYPE key_word_;
            std::string symbol_;
            std::string iden_;
            std::string strval_;
            int intval_;
        } parse_value_;
    };
    
} // namespace jack_compiler
