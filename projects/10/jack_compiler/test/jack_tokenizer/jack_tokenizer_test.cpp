#include <iostream>
#include <fstream>
#include "gtest/gtest.h"

#include "jack_tokenizer/jack_tokenizer.hpp"

namespace jack_compiler
{
    TEST(MainTest, CommentForLine) {
        std::stringbuf sb("//uuuuu/ssss*....asdgb\n");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(MainTest, CommentForNormal) {
        std::stringbuf sb("/*uuu  u/ss class ss*.5...asdgb*/");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(MainTest, CommentForNormalNoSlash) {
        std::stringbuf sb("/*uuu  u/ss class ss*.5...asdgb*");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(MainTest, CommentForDocument) {
        std::stringbuf sb("/**uuu  u/ss class ss*.5...asdgb*/");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(MainTest, Symbol) {
        std::stringbuf sb("()");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("(", jack.GetSymbol());
        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ(")", jack.GetSymbol());
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(MainTest, Interger) {
        std::stringbuf sb("//asd\n-13");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("-", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::INT_CONST, jack.GetTokenType());
        ASSERT_EQ(13, jack.GetIntVal());
        jack.Advance();
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(MainTest, IntergerMulti) {
        std::stringbuf sb("-13+26=5");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("-", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::INT_CONST, jack.GetTokenType());
        ASSERT_EQ(13, jack.GetIntVal());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("+", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::INT_CONST, jack.GetTokenType());
        ASSERT_EQ(26, jack.GetIntVal());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("=", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::INT_CONST, jack.GetTokenType());
        ASSERT_EQ(5, jack.GetIntVal());
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(MainTest, String) {
        std::stringbuf sb("\"abc\"+\"zzzz\n");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::STRING_CONST, jack.GetTokenType());
        ASSERT_EQ("abc", jack.GetStringVal());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("+", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::STRING_CONST, jack.GetTokenType());
        ASSERT_EQ("zzzz", jack.GetStringVal());
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(MainTest, Keyword) {
        std::stringbuf sb("function {\nreturn null;\n}");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::KEYWORD, jack.GetTokenType());
        ASSERT_EQ(KEYWORD_TYPE::FUNCTION, jack.GetTokenType());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("{", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::KEYWORD, jack.GetTokenType());
        ASSERT_EQ(KEYWORD_TYPE::RETURN, jack.GetTokenType());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::KEYWORD, jack.GetTokenType());
        ASSERT_EQ(KEYWORD_TYPE::NULL_, jack.GetTokenType());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ(";", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("}", jack.GetSymbol());
        ASSERT_FALSE(jack.HasMoreTokens());
    }
} // namespace jack_compiler
