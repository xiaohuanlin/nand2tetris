#pragma once

#include <iostream>
#include "utils/enum/enum.hpp"

namespace jack_compiler {
    class JackTokenizer {
    public:
        JackTokenizer(std::istream* input);
        // JackTokenizer(const std::string file);

        /**
         * If there are more tokens?
         */
        bool HasMoreTokens();

        /**
         * Only if there are more tokens, we can use this function to next token
         */
        void Advance();

        inline TOKEN_TYPE GetTokenType() const noexcept {return token_type_;};
        inline KEYWORD_TYPE GetKeyword() const noexcept {return parse_value_.key_word_;};
        inline std::string GetSymbol() const noexcept {return parse_value_.symbol_;};
        inline std::string GetIdentifier() const noexcept {return parse_value_.iden_;};
        inline int GetIntVal() const noexcept {return parse_value_.intval_;};
        inline std::string GetStringVal() const noexcept {return parse_value_.strval_;};
    private:
        /**
         * Judge the token is right or should be passed
         */
        bool IsValidToken(std::string token);
        std::istream* input_;
        TOKEN_TYPE token_type_;
        union ParseValue {
            KEYWORD_TYPE key_word_;
            std::string symbol_;
            std::string iden_;
            std::string strval_;
            int intval_;
            ParseValue() {
                key_word_ = KEYWORD_TYPE::NULL_;
                symbol_ = "";
                iden_ = "";
                strval_ = "";
            };
            ~ParseValue() {}
        } parse_value_;
    };
    
} // namespace jack_compiler
