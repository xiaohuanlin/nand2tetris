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
enum class TerminalTokenType {
  kUnspecify,  // dummy type
  kKeyword,
  kSymbol,
  kIdentifier,
  kIntConst,
  kStringConst,
};

static const std::unordered_map<TerminalTokenType, std::string>
    TerminalTokenTypeString {
        {TerminalTokenType::kKeyword, "keyword"},
        {TerminalTokenType::kSymbol, "symbol"},
        {TerminalTokenType::kIdentifier, "identifier"},
        {TerminalTokenType::kIntConst, "integerConstant"},
        {TerminalTokenType::kStringConst, "stringConstant"},
    };

/**
 * All of the non-terminal token type
 */
enum class NonTerminalTokenType {
  kUnspecify,  // dummy type
  kTokens,     // dummy type
  kClass,
  kClassVarDec,
  kSubroutineDec,
  kParameterList,
  kSubroutineBody,
  kVarDec,
  kStatements,
  kWhileStatement,
  kIfStatement,
  kReturnStatement,
  kLetStatement,
  kDoStatement,
  kExpression,
  kTerm,
  kExpressionList
};

static const std::unordered_map<NonTerminalTokenType, std::string>
    NonTerminalTokenTypeString {
        {NonTerminalTokenType::kClass, "class"},
        {NonTerminalTokenType::kTokens, "tokens"},
        {NonTerminalTokenType::kClassVarDec, "classVarDec"},
        {NonTerminalTokenType::kSubroutineDec, "subroutineDec"},
        {NonTerminalTokenType::kParameterList, "parameterList"},
        {NonTerminalTokenType::kSubroutineBody, "subroutineBody"},
        {NonTerminalTokenType::kVarDec, "varDec"},
        {NonTerminalTokenType::kStatements, "statements"},
        {NonTerminalTokenType::kWhileStatement, "whileStatement"},
        {NonTerminalTokenType::kIfStatement, "ifStatement"},
        {NonTerminalTokenType::kReturnStatement, "returnStatement"},
        {NonTerminalTokenType::kLetStatement, "letStatement"},
        {NonTerminalTokenType::kDoStatement, "doStatement"},
        {NonTerminalTokenType::kExpression, "expression"},
        {NonTerminalTokenType::kTerm, "term"},
        {NonTerminalTokenType::kExpressionList, "expressionList"},
    };

/**
 * All of the keyword type
 */
enum class KeywordType {
  kClass,
  kMethod,
  kInt,
  kFunction,
  kBoolean,
  kConstructor,
  kChar,
  kVoid,
  kVar,
  kStatic,
  kField,
  kLet,
  kDo,
  kIf,
  kElse,
  kWhile,
  kReturn,
  kTrue,
  kFalse,
  kNull,
  kThis,
};

static const std::unordered_set<char> SymbolSet{
    '(', ')', '{', '}', '[', ']', ',', ';', '=', '.',
    '+', '-', '*', '/', '&', '|', '<', '>', '~'};

static const std::unordered_map<std::string, KeywordType> StringKeywordType{
    {"class", KeywordType::kClass},
    {"constructor", KeywordType::kConstructor},
    {"method", KeywordType::kMethod},
    {"function", KeywordType::kFunction},
    {"int", KeywordType::kInt},
    {"boolean", KeywordType::kBoolean},
    {"char", KeywordType::kChar},
    {"void", KeywordType::kVoid},
    {"var", KeywordType::kVar},
    {"static", KeywordType::kStatic},
    {"field", KeywordType::kField},
    {"let", KeywordType::kLet},
    {"do", KeywordType::kDo},
    {"if", KeywordType::kIf},
    {"else", KeywordType::kElse},
    {"while", KeywordType::kWhile},
    {"return", KeywordType::kReturn},
    {"true", KeywordType::kTrue},
    {"false", KeywordType::kFalse},
    {"null", KeywordType::kNull},
    {"this", KeywordType::kThis},
};

static const std::unordered_map<KeywordType, std::string> KeywordTypeString{
    {KeywordType::kClass, "class"},
    {KeywordType::kConstructor, "constructor"},
    {KeywordType::kMethod, "method"},
    {KeywordType::kFunction, "function"},
    {KeywordType::kInt, "int"},
    {KeywordType::kBoolean, "boolean"},
    {KeywordType::kChar, "char"},
    {KeywordType::kVoid, "void"},
    {KeywordType::kVar, "var"},
    {KeywordType::kStatic, "static"},
    {KeywordType::kField, "field"},
    {KeywordType::kLet, "let"},
    {KeywordType::kDo, "do"},
    {KeywordType::kIf, "if"},
    {KeywordType::kElse, "else"},
    {KeywordType::kWhile, "while"},
    {KeywordType::kReturn, "return"},
    {KeywordType::kTrue, "true"},
    {KeywordType::kFalse, "false"},
    {KeywordType::kNull, "null"},
    {KeywordType::kThis, "this"},
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
    {"<", OpType::kLt},
    {">", OpType::kGt},
    {"&", OpType::kAnd},
    {"|", OpType::kOr},
};

}  // namespace jack_compiler

#endif  // JACK_COMPILER_ENUM_H_
