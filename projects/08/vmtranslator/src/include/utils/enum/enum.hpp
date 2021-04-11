#pragma once

#include <iostream>
#include <unordered_map>

namespace vmtranslator {
    /**
     * The size of reading command
     */
    const static std::streamsize BUFFER_SIZE = 256;

    /**
     * The command type for all commands
     */
    enum class COMMAND {
        C_ARITHMETIC,
        C_PUSH,
        C_POP,
        C_LABEL,
        C_GOTO,
        C_IF,
        C_FUNCTION,
        C_RETURN,
        C_CALL,
    };

    const static std::unordered_map<std::string, COMMAND> STRING_COMMAND {
        {"add",     COMMAND::C_ARITHMETIC},
        {"sub",     COMMAND::C_ARITHMETIC},
        {"neg",     COMMAND::C_ARITHMETIC},
        {"eq",      COMMAND::C_ARITHMETIC},
        {"gt",      COMMAND::C_ARITHMETIC},
        {"lt",      COMMAND::C_ARITHMETIC},
        {"and",     COMMAND::C_ARITHMETIC},
        {"or",      COMMAND::C_ARITHMETIC},
        {"not",     COMMAND::C_ARITHMETIC},
        {"push",    COMMAND::C_PUSH},
        {"pop",     COMMAND::C_POP},
        {"if-goto", COMMAND::C_IF},
        {"goto",    COMMAND::C_GOTO},
        {"label",   COMMAND::C_LABEL},
        {"function",COMMAND::C_FUNCTION},
        {"return",  COMMAND::C_RETURN},
        {"call",    COMMAND::C_CALL},
    };

    /**
     * Types for arithmetic command
     */
    enum class ARITHMETIC_TYPE {
        NEG,
        NOT,
        ADD,
        SUB,
        EQ,
        GT,
        LT,
        AND,
        OR
    };

    const static std::unordered_map<std::string, ARITHMETIC_TYPE> STRING_ARITHMETIC_TYPE {
        {"neg",     ARITHMETIC_TYPE::NEG},
        {"not",     ARITHMETIC_TYPE::NOT},
        {"add",     ARITHMETIC_TYPE::ADD},
        {"sub",     ARITHMETIC_TYPE::SUB},
        {"eq",      ARITHMETIC_TYPE::EQ},
        {"gt",      ARITHMETIC_TYPE::GT},
        {"lt",      ARITHMETIC_TYPE::LT},
        {"and",     ARITHMETIC_TYPE::AND},
        {"or",      ARITHMETIC_TYPE::OR},
    };

    /**
     * The name of different area in memory 
     */
    enum class MEMORY_AREA {
        SP,
        ARGUMENT,
        LOCAL,
        STATIC,
        CONSTANT,
        THIS,
        THAT,
        POINTER,
        TEMP,
        GENERAL_USE_1,
        GENERAL_USE_2,
    };

    const static std::unordered_map<std::string, MEMORY_AREA> STRING_MEMORY_AREA {
        {"sp",      MEMORY_AREA::SP},
        {"local",   MEMORY_AREA::LOCAL},
        {"argument",MEMORY_AREA::ARGUMENT},
        {"static",  MEMORY_AREA::STATIC},
        {"constant",MEMORY_AREA::CONSTANT},
        {"this",    MEMORY_AREA::THIS},
        {"that",    MEMORY_AREA::THAT},
        {"pointer", MEMORY_AREA::POINTER},
        {"temp",    MEMORY_AREA::TEMP},
    };

    /**
     * The initial address for stack pointer
     */
    const static std::string SP_INIT_ADDRESS = "@256";

    /**
     * The start(or only) address for all segement areas
     */
    const static std::unordered_map<MEMORY_AREA, int16_t> SEGMENT_BEGIN_ADDRESS {
        {MEMORY_AREA::SP,           0},
        {MEMORY_AREA::LOCAL,        1},
        {MEMORY_AREA::ARGUMENT,     2},
        {MEMORY_AREA::THIS,         3},
        {MEMORY_AREA::POINTER,      3},
        {MEMORY_AREA::THAT,         4},
        {MEMORY_AREA::TEMP,         5},
        {MEMORY_AREA::GENERAL_USE_1,13},
        {MEMORY_AREA::GENERAL_USE_2,14},
        {MEMORY_AREA::STATIC,       16},
    }; 
} // namespace vmtranslator