#include <algorithm>
#include <iostream>
#include <fstream>
#include "gtest/gtest.h"

#include "jack_tokenizer/jack_tokenizer.h"

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
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("(", jack.GetSymbol());
        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ(")", jack.GetSymbol());
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(TokenizerTest, Interger) {
        std::stringbuf sb("//asd\n-13");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("-", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::INT_CONST, jack.GetTokenType());
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
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("-", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::INT_CONST, jack.GetTokenType());
        ASSERT_EQ(13, jack.GetIntVal());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("+", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::INT_CONST, jack.GetTokenType());
        ASSERT_EQ(26, jack.GetIntVal());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("=", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::INT_CONST, jack.GetTokenType());
        ASSERT_EQ(5, jack.GetIntVal());
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(TokenizerTest, String) {
        std::stringbuf sb("\"abc\"+\"zzzz\n");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::STRING_CONST, jack.GetTokenType());
        ASSERT_EQ("abc", jack.GetStringVal());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("+", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::STRING_CONST, jack.GetTokenType());
        ASSERT_EQ("zzzz", jack.GetStringVal());
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(TokenizerTest, Keyword) {
        std::stringbuf sb("function {\nreturn null;\n}");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::KEYWORD, jack.GetTokenType());
        ASSERT_EQ(static_cast<int>(KEYWORD_TYPE::FUNCTION), static_cast<int>(jack.GetKeyword()));
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("{", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::KEYWORD, jack.GetTokenType());
        ASSERT_EQ(static_cast<int>(KEYWORD_TYPE::RETURN), static_cast<int>(jack.GetKeyword()));
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::KEYWORD, jack.GetTokenType());
        ASSERT_EQ(static_cast<int>(KEYWORD_TYPE::NULL_), static_cast<int>(jack.GetKeyword()));
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ(";", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("}", jack.GetSymbol());
        ASSERT_FALSE(jack.HasMoreTokens());
    }

    TEST(TokenizerTest, Identifier) {
        std::stringbuf sb("first=15+first\nsecond");
        std::istream is(&sb);
        JackTokenizer jack(&is);

        ASSERT_TRUE(jack.HasMoreTokens());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::IDENTIFIER, jack.GetTokenType());
        ASSERT_EQ("first", jack.GetIdentifier());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("=", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::INT_CONST, jack.GetTokenType());
        ASSERT_EQ(15, jack.GetIntVal());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::SYMBOL, jack.GetTokenType());
        ASSERT_EQ("+", jack.GetSymbol());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::IDENTIFIER, jack.GetTokenType());
        ASSERT_EQ("first", jack.GetIdentifier());
        jack.Advance();
        ASSERT_EQ(TERMINAL_TOKEN_TYPE::IDENTIFIER, jack.GetTokenType());
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

    void RunTestByCase(char* directory, char* test_name) {
        char check_file_name[BUFFER_SIZE];
        char template_str[] = "../../%s/%s.jack";
        snprintf(check_file_name, BUFFER_SIZE, template_str, directory, test_name);

        char result_file_name[BUFFER_SIZE];
        char result_template_str[] = "../../%s/%sT.xml";
        snprintf(result_file_name, BUFFER_SIZE, result_template_str, directory, test_name);

        std::string test_file_name = "./test.xml";

        JackTokenizer jack(check_file_name);
        XMLWriter writer(test_file_name);
        jack.CreateOutput(&writer);

        // read actual result output
        std::ifstream actual_result(result_file_name);
        actual_result.seekg(0, actual_result.end);
        std::streamsize length = actual_result.tellg();
        actual_result.seekg(0, actual_result.beg);

        char actual_result_buffer[length + 1];
        actual_result.read(actual_result_buffer, length);
        actual_result_buffer[length] = '\0';

        // replace all \n\r ' ' \t
        std::string actual_result_str = std::string(actual_result_buffer);
        int count = 0;
        std::remove_if(actual_result_str.begin(), actual_result_str.end(), [&] (char& v) {
            if (v == '\n' || v == '\r' || v == ' ' || '\t') {
                ++count;
                return true;
            }
            return false;
        });
        actual_result_str.resize(actual_result_str.length() - count);

        // read test output
        std::ifstream test_result(test_file_name);
        test_result.seekg(0, test_result.end);
        length = test_result.tellg();
        test_result.seekg(0, test_result.beg);

        char buffer[length + 1];
        test_result.read(buffer, length);
        buffer[length] = '\0';

        // replace all \n\r ' ' \t
        std::string test_result_str = std::string(buffer);
        count = 0;
        std::remove_if(test_result_str.begin(), test_result_str.end(), [&] (char& v) {
            if (v == '\n' || v == '\r' || v == ' ' || '\t') {
                ++count;
                return true;
            }
            return false;
        });
        test_result_str.resize(test_result_str.length() - count);
        ASSERT_EQ(actual_result_str, test_result_str);
        unlink(test_file_name.c_str());
    }

    TEST(TokenizerTest, ArrayMainXMLTokenOutput) {
        char dir_name[] = "ArrayTest";
        char file_name[] = "Main";
        RunTestByCase(dir_name, file_name);
    }

    TEST(TokenizerTest, ExpressionLessSquareMainXMLTokenOutput) {
        char dir_name[] = "ExpressionLessSquare";
        char file_name[] = "Main";
        RunTestByCase(dir_name, file_name);
    }

    TEST(TokenizerTest, ExpressionLessSquareSquareXMLTokenOutput) {
        char dir_name[] = "ExpressionLessSquare";
        char file_name[] = "Square";
        RunTestByCase(dir_name, file_name);
    }

    TEST(TokenizerTest, ExpressionLessSquareSquareGameXMLTokenOutput) {
        char dir_name[] = "ExpressionLessSquare";
        char file_name[] = "SquareGame";
        RunTestByCase(dir_name, file_name);
    }

    TEST(TokenizerTest, SquareMainXMLTokenOutput) {
        char dir_name[] = "Square";
        char file_name[] = "Main";
        RunTestByCase(dir_name, file_name);
    }

    TEST(TokenizerTest, SquareSquareXMLTokenOutput) {
        char dir_name[] = "Square";
        char file_name[] = "Square";
        RunTestByCase(dir_name, file_name);
    }

    TEST(TokenizerTest, SquareSquareGameXMLTokenOutput) {
        char dir_name[] = "Square";
        char file_name[] = "SquareGame";
        RunTestByCase(dir_name, file_name);
    }
}  // namespace jack_compiler
