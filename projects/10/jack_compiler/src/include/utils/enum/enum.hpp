#pragma once

#include <iostream>
#include <unordered_map>

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
    }

} // namespace jack_compiler