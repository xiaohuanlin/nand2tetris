#ifndef JACK_COMPILER_ENUM_H_
#define JACK_COMPILER_ENUM_H_

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace jack_compiler {
/**
 * The size of reading token
 */
constexpr static std::streamsize BUFFER_SIZE = 256;

/**
 * All of the terminal token type
 */
enum class TERMINAL_TOKEN_TYPE {
  UNSPECIFY,  // dummy type
  KEYWORD,
  SYMBOL,
  IDENTIFIER,
  INT_CONST,
  STRING_CONST,
};

static const std::unordered_map<TERMINAL_TOKEN_TYPE, std::string>
    TERMINAL_TOKEN_TABLE{
        {TERMINAL_TOKEN_TYPE::KEYWORD, "keyword"},
        {TERMINAL_TOKEN_TYPE::SYMBOL, "symbol"},
        {TERMINAL_TOKEN_TYPE::IDENTIFIER, "identifier"},
        {TERMINAL_TOKEN_TYPE::INT_CONST, "integerConstant"},
        {TERMINAL_TOKEN_TYPE::STRING_CONST, "stringConstant"},
    };

/**
 * All of the non-terminal token type
 */
enum class NON_TERMINAL_TOKEN_TYPE {
  UNSPECIFY,  // dummy type
  TOKENS,     // dummy type
  CLASS,
  CLASS_VAR_DEC,
  SUBROUTINE_DEC,
  PARAMETER_LIST,
  SUBROUTINE_BODY,
  VARDEC,
  STATEMENTS,
  WHILE_STATEMENT,
  IF_STATEMENT,
  RETURN_STATEMENT,
  LET_STATEMENT,
  DO_STATEMENT,
  EXPRESSION,
  TERM,
  EXPRESSION_LIST
};

static const std::unordered_map<NON_TERMINAL_TOKEN_TYPE, std::string>
    NON_TERMINAL_TOKEN_TABLE{
        {NON_TERMINAL_TOKEN_TYPE::CLASS, "class"},
        {NON_TERMINAL_TOKEN_TYPE::TOKENS, "tokens"},
        {NON_TERMINAL_TOKEN_TYPE::CLASS_VAR_DEC, "classVarDec"},
        {NON_TERMINAL_TOKEN_TYPE::SUBROUTINE_DEC, "subroutineDec"},
        {NON_TERMINAL_TOKEN_TYPE::PARAMETER_LIST, "parameterList"},
        {NON_TERMINAL_TOKEN_TYPE::SUBROUTINE_BODY, "subroutineBody"},
        {NON_TERMINAL_TOKEN_TYPE::VARDEC, "varDec"},
        {NON_TERMINAL_TOKEN_TYPE::STATEMENTS, "statements"},
        {NON_TERMINAL_TOKEN_TYPE::WHILE_STATEMENT, "whileStatement"},
        {NON_TERMINAL_TOKEN_TYPE::IF_STATEMENT, "ifStatement"},
        {NON_TERMINAL_TOKEN_TYPE::RETURN_STATEMENT, "returnStatement"},
        {NON_TERMINAL_TOKEN_TYPE::LET_STATEMENT, "letStatement"},
        {NON_TERMINAL_TOKEN_TYPE::DO_STATEMENT, "doStatement"},
        {NON_TERMINAL_TOKEN_TYPE::EXPRESSION, "expression"},
        {NON_TERMINAL_TOKEN_TYPE::TERM, "term"},
        {NON_TERMINAL_TOKEN_TYPE::EXPRESSION_LIST, "expressionList"},
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

static const std::unordered_set<char> SYMBOL_TABLE{
    '(', ')', '{', '}', '[', ']', ',', ';', '=', '.',
    '+', '-', '*', '/', '&', '|', '<', '>', '~'};

static const std::unordered_map<std::string, KEYWORD_TYPE> KEYWORD_TABLE{
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

static const std::unordered_map<KEYWORD_TYPE, std::string> KEYWORD_STR_TABLE{
    {KEYWORD_TYPE::CLASS, "class"},
    {KEYWORD_TYPE::CONSTRUCTOR, "constructor"},
    {KEYWORD_TYPE::METHOD, "method"},
    {KEYWORD_TYPE::FUNCTION, "function"},
    {KEYWORD_TYPE::INT, "int"},
    {KEYWORD_TYPE::BOOLEAN, "boolean"},
    {KEYWORD_TYPE::CHAR, "char"},
    {KEYWORD_TYPE::VOID, "void"},
    {KEYWORD_TYPE::VAR, "var"},
    {KEYWORD_TYPE::STATIC, "static"},
    {KEYWORD_TYPE::FIELD, "field"},
    {KEYWORD_TYPE::LET, "let"},
    {KEYWORD_TYPE::DO, "do"},
    {KEYWORD_TYPE::IF, "if"},
    {KEYWORD_TYPE::ELSE, "else"},
    {KEYWORD_TYPE::WHILE, "while"},
    {KEYWORD_TYPE::RETURN, "return"},
    {KEYWORD_TYPE::TRUE, "true"},
    {KEYWORD_TYPE::FALSE, "false"},
    {KEYWORD_TYPE::NULL_, "null"},
    {KEYWORD_TYPE::THIS, "this"},
};

enum class SymbolTableKind {
    kStatic,
    kField,
    kArg,
    kVar,
};

enum class SegmentType {
    kConst,
    kArg,
    kLocal,
    kStatic,
    kThis,
    kThat,
    kPointer,
    kTemp,
};

const static std::unordered_map<SymbolTableKind, SegmentType> SymbolTableKindToSegmentType {
    {SymbolTableKind::kStatic, SegmentType::kStatic},
    {SymbolTableKind::kField, SegmentType::kThis},
    {SymbolTableKind::kArg, SegmentType::kArg},
    {SymbolTableKind::kVar, SegmentType::kLocal},
};

const static std::unordered_map<SegmentType, std::string> SegmentTypeString {
    {SegmentType::kConst,   "constant"},
    {SegmentType::kArg,     "argument"},
    {SegmentType::kLocal,   "local"},
    {SegmentType::kStatic,  "static"},
    {SegmentType::kThis,    "this"},
    {SegmentType::kThat,    "that"},
    {SegmentType::kPointer, "pointer"},
    {SegmentType::kTemp,    "temp"},
};


enum class ArithmeticType {
    kAdd,
    kSub,
    kNeg,
    kEq,
    kGt,
    kLt,
    kAnd,
    kOr,
    kNot
};

const static std::unordered_map<ArithmeticType, std::string> ArithmeticTypeString {
    {ArithmeticType::kAdd,  "add"},
    {ArithmeticType::kSub,  "sub"},
    {ArithmeticType::kNeg,  "neg"},
    {ArithmeticType::kEq,   "eq"},
    {ArithmeticType::kGt,   "gt"},
    {ArithmeticType::kLt,   "lt"},
    {ArithmeticType::kAnd,  "and"},
    {ArithmeticType::kOr,   "or"},
    {ArithmeticType::kNot,  "not"},
};

enum class OpType {
    kAdd,
    kSub,
    kMultiply,
    kDivide,
    kEq,
    kGt,
    kLt,
    kAnd,
    kOr,
};

const static std::unordered_map<std::string, OpType> StringOpType {
    {"+", OpType::kAdd},
    {"-", OpType::kSub},
    {"*", OpType::kMultiply},
    {"/", OpType::kDivide},
    {"=", OpType::kEq},
    {"<", OpType::kGt},
    {">", OpType::kLt},
    {"&", OpType::kAnd},
    {"|", OpType::kOr},
};

}  // namespace jack_compiler

#endif  // JACK_COMPILER_ENUM_H_
