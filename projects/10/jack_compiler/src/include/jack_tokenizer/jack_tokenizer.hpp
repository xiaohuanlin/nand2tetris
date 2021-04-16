#pragma once

#include <iostream>
#include "utils/enum/enum.hpp"

namespace jack_compiler {
    class JackTokenizer {
    public:
        explicit JackTokenizer(std::istream* input);
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
        inline KEYWORD_TYPE GetKeyword() const noexcept {return key_word_;};
        inline std::string GetSymbol() const noexcept {return symbol_;};
        inline std::string GetIdentifier() const noexcept {return identifier_;};
        inline int GetIntVal() const noexcept {return intval_;};
        inline std::string GetStringVal() const noexcept {return strval_;};
    private:
        /**
         * Judge the token is right or should be passed
         */
        bool IsValidToken(char c);
        bool IsValidComment(char c);
        bool IsValidSymbol(char c);
        bool IsValidInteger(char c);
        bool IsValidString(char c);
        bool IsValidIdentifier(char c);
        bool IsValidKeyWord(const std::string& keyword);
        std::istream* input_;
        TOKEN_TYPE token_type_;
        KEYWORD_TYPE key_word_;
        std::string symbol_;
        std::string identifier_;
        std::string strval_;
        int intval_;
    };
    
} // namespace jack_compiler
