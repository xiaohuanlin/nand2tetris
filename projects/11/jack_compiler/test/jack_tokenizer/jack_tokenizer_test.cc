#include <algorithm>
#include <iostream>
#include <fstream>
#include "gtest/gtest.h"

#include "jack_tokenizer/jack_tokenizer.h"
#include "writer/xml/xml_writer.h"

namespace jack_compiler {
    TEST(TokenizerTest, CommentForLine) {
        std::stringbuf sb("//uuuuu/ssss*....asdgb\n");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(TokenizerTest, CommentForNormal) {
        std::stringbuf sb("/*uuu  u/ss class ss*.5...asdgb*/");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(TokenizerTest, CommentForNormalNoSlash) {
        std::stringbuf sb("/*uuu  u/ss class ss*.5...asdgb*");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(TokenizerTest, CommentForDocument) {
        std::stringbuf sb("/**uuu  u/ss class ss*.5...asdgb*/");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(TokenizerTest, Symbol) {
        std::stringbuf sb("()");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kSymbol, jack.GetTokenType());
        ASSERT_EQ("(", jack.GetSymbol());
        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kSymbol, jack.GetTokenType());
        ASSERT_EQ(")", jack.GetSymbol());
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(TokenizerTest, Interger) {
        std::stringbuf sb("//asd\n-13");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kSymbol, jack.GetTokenType());
        ASSERT_EQ("-", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kIntConst, jack.GetTokenType());
        ASSERT_EQ(13, jack.GetIntVal());
        jack.Advance();
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(TokenizerTest, IntergerMulti) {
        std::stringbuf sb("-13+26=5");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kSymbol, jack.GetTokenType());
        ASSERT_EQ("-", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kIntConst, jack.GetTokenType());
        ASSERT_EQ(13, jack.GetIntVal());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kSymbol, jack.GetTokenType());
        ASSERT_EQ("+", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kIntConst, jack.GetTokenType());
        ASSERT_EQ(26, jack.GetIntVal());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kSymbol, jack.GetTokenType());
        ASSERT_EQ("=", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kIntConst, jack.GetTokenType());
        ASSERT_EQ(5, jack.GetIntVal());
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(TokenizerTest, String) {
        std::stringbuf sb("\"abc\"+\"zzzz\n");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kStringConst, jack.GetTokenType());
        ASSERT_EQ("abc", jack.GetStringVal());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kSymbol, jack.GetTokenType());
        ASSERT_EQ("+", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kStringConst, jack.GetTokenType());
        ASSERT_EQ("zzzz", jack.GetStringVal());
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(TokenizerTest, Keyword) {
        std::stringbuf sb("function {\nreturn null;\n}");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kKeyword, jack.GetTokenType());
        ASSERT_EQ(static_cast<int>(KeywordType::kFunction), static_cast<int>(jack.GetKeyword()));
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kSymbol, jack.GetTokenType());
        ASSERT_EQ("{", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kKeyword, jack.GetTokenType());
        ASSERT_EQ(static_cast<int>(KeywordType::kReturn), static_cast<int>(jack.GetKeyword()));
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kKeyword, jack.GetTokenType());
        ASSERT_EQ(static_cast<int>(KeywordType::kNull), static_cast<int>(jack.GetKeyword()));
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kSymbol, jack.GetTokenType());
        ASSERT_EQ(";", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kSymbol, jack.GetTokenType());
        ASSERT_EQ("}", jack.GetSymbol());
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(TokenizerTest, Identifier) {
        std::stringbuf sb("first=15+first\nsecond");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kIdentifier, jack.GetTokenType());
        ASSERT_EQ("first", jack.GetIdentifier());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kSymbol, jack.GetTokenType());
        ASSERT_EQ("=", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kIntConst, jack.GetTokenType());
        ASSERT_EQ(15, jack.GetIntVal());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kSymbol, jack.GetTokenType());
        ASSERT_EQ("+", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kIdentifier, jack.GetTokenType());
        ASSERT_EQ("first", jack.GetIdentifier());
        jack.Advance();
        ASSERT_EQ(TerminalTokenType::kIdentifier, jack.GetTokenType());
        ASSERT_EQ("second", jack.GetIdentifier());
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(TokenizerTest, XMLOutput) {
        std::stringbuf sb("function {\nreturn null;\n}");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        std::string output_file = "./test.xml";
        XMLWriter writer(output_file);
        jack.CreateOutput(&writer);
        std::ifstream input(output_file);
        input.seekg(0, input.end);
        std::streamsize length = input.tellg();
        input.seekg(0, input.beg);

        char buffer[length + 1];
        input.read(buffer, length);
        buffer[length] = '\0';
        ASSERT_EQ(
            "<tokens>\n"
                "\t<keyword> function </keyword>\n"
                "\t<symbol> { </symbol>\n"
                "\t<keyword> return </keyword>\n"
                "\t<keyword> null </keyword>\n"
                "\t<symbol> ; </symbol>\n"
                "\t<symbol> } </symbol>\n"
            "</tokens>\n",
            std::string(buffer));
        unlink(output_file.c_str());
    }
}  // namespace jack_compiler
