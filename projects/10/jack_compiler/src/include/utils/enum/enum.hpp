#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>

namespace jack_compiler {
    /**
     * The size of reading token
     */
    const static std::streamsize BUFFER_SIZE = 256;

    /**
     * All of the token type
     */
    enum class TOKEN_TYPE {
        KEYWORD,
        SYMBOL,
        IDENTIFIER,
        INT_CONST,
        STRING_CONST,
    };

    /**
     * All of the keyword type
     */
    enum class KEYWORD_TYPE {
        CLASS,
        METHOD,
        INT,
        FUNCTION,
        BOOLEAN,
        CONSTRUCTOR,
        CHAR,
        VOID,
        VAR,
        STATIC,
        FIELD,
        LET,
        DO,
        IF,
        ELSE,
        WHILE,
        RETURN,
        TRUE,
        FALSE,
        NULL_,
        THIS,
    };

    const static std::unordered_set<char> SYMBOL_TABLE {
        '(', ')', '{', '}', '[', ']',
        ',', ';', '=', '.', '+', '-',
        '*', '/', '&', '|', '<', '>',
    };

    const static std::unordered_map<std::string, KEYWORD_TYPE> KEYWORD_TABLE {
        {"class", KEYWORD_TYPE::CLASS},
        {"constructor", KEYWORD_TYPE::CONSTRUCTOR},
        {"method", KEYWORD_TYPE::METHOD},
        {"function", KEYWORD_TYPE::FUNCTION},
        {"int", KEYWORD_TYPE::INT},
        {"boolean", KEYWORD_TYPE::BOOLEAN},
        {"char", KEYWORD_TYPE::CHAR},
        {"void", KEYWORD_TYPE::VOID},
        {"var", KEYWORD_TYPE::VAR},
        {"static", KEYWORD_TYPE::STATIC},
        {"field", KEYWORD_TYPE::FIELD},
        {"let", KEYWORD_TYPE::LET},
        {"do", KEYWORD_TYPE::DO},
        {"if", KEYWORD_TYPE::IF},
        {"else", KEYWORD_TYPE::ELSE},
        {"while", KEYWORD_TYPE::WHILE},
        {"return", KEYWORD_TYPE::RETURN},
        {"true", KEYWORD_TYPE::TRUE},
        {"false", KEYWORD_TYPE::FALSE},
        {"null", KEYWORD_TYPE::NULL_},
        {"this", KEYWORD_TYPE::THIS},
    };

} // namespace jack_compiler