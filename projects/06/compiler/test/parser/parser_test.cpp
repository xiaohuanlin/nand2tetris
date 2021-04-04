#include "parser.hpp"
#include <string>

#include "gtest/gtest.h"

namespace compiler
{
    TEST(ParserTest, CreateFromFile) {
        const std::string name = "test";
        auto p = new Parser(name);
        EXPECT_TRUE(p != nullptr);
    }

    TEST(ParserTest, MoreCommand) {
        std::stringbuf sb;
        std::string test_word = "@2\n";
        sb.sputn(test_word.c_str(), test_word.length() + 1);
        std::istream in(&sb);
        Parser p(&in);
        ASSERT_TRUE(p.HasMoreCommands());
        p.Advance();
        ASSERT_TRUE(p.HasMoreCommands());
        p.Advance();
        ASSERT_FALSE(p.HasMoreCommands());
    }

    TEST(ParserTest, EmptyCommand) {
        std::stringbuf sb;
        std::string test_word = "";
        sb.sputn(test_word.c_str(), test_word.length() + 1);
        std::istream in(&sb);
        Parser p(&in);
        p.Advance();
        ASSERT_FALSE(p.HasMoreCommands());
    }

    TEST(ParserTest, CommentCommand) {
        std::stringbuf sb;
        std::string test_word = "  //@2\nD=M  //easy command";
        sb.sputn(test_word.c_str(), test_word.length() + 1);
        std::istream in(&sb);
        Parser p(&in);
        p.Advance();

        ASSERT_EQ(p.GetCommandType(), COMMAND::C_COMMAND);
        ASSERT_EQ(p.GetDest(), "D");
        ASSERT_EQ(p.GetComp(), "M");
        ASSERT_FALSE(p.HasMoreCommands());
    }

    TEST(ParserTest, Advance) {
        std::stringbuf sb;
        std::string test_word = "@2\nD=D+M";
        sb.sputn(test_word.c_str(), test_word.length() + 1);
        std::istream in(&sb);
        Parser p(&in);
        ASSERT_TRUE(p.HasMoreCommands());
        p.Advance();
        ASSERT_TRUE(p.HasMoreCommands());
        p.Advance();
        ASSERT_FALSE(p.HasMoreCommands());
    }

    TEST(ParserTest, ACommand) {
        std::stringbuf sb;
        std::string test_word = "@2\r\n@R0\r\n@THIS";
        sb.sputn(test_word.c_str(), test_word.length() + 1);
        std::istream in(&sb);
        Parser p(&in);
        ASSERT_TRUE(p.HasMoreCommands());
        p.Advance();

        ASSERT_EQ(p.GetCommandType(), COMMAND::A_COMMAND);
        ASSERT_EQ(p.GetSymbol(), "2");

        p.Advance();

        ASSERT_EQ(p.GetCommandType(), COMMAND::A_COMMAND);
        ASSERT_EQ(p.GetSymbol(), "R0");

        p.Advance();

        ASSERT_EQ(p.GetCommandType(), COMMAND::A_COMMAND);
        ASSERT_EQ(p.GetSymbol(), "THIS");
    }

    TEST(ParserTest, ErrorACommand) {
        std::stringbuf sb;
        std::string test_word = "@asxxx235kk?sda\n@xyz12:._";
        sb.sputn(test_word.c_str(), test_word.length() + 1);
        std::istream in(&sb);
        Parser p(&in);
        ASSERT_TRUE(p.HasMoreCommands());

        p.Advance();
        //Skip error command
        ASSERT_EQ(p.GetCommandType(), COMMAND::A_COMMAND);
        ASSERT_EQ(p.GetSymbol(), "xyz12:._");
    }

    TEST(ParserTest, LCommand) {
        std::stringbuf sb;
        std::string test_word = "(xz)";
        sb.sputn(test_word.c_str(), test_word.length() + 1);
        std::istream in(&sb);
        Parser p(&in);
        ASSERT_TRUE(p.HasMoreCommands());
        p.Advance();

        ASSERT_EQ(p.GetCommandType(), COMMAND::L_COMMAND);
        ASSERT_EQ(p.GetSymbol(), "xz");
    }

    TEST(ParserTest, CCommand) {
        std::stringbuf sb;
        std::string test_word = "D=D+M\nM=!M+M\nAM=!M;JMP\nA-D;JEQ";
        sb.sputn(test_word.c_str(), test_word.length() + 1);
        std::istream in(&sb);
        Parser p(&in);
        ASSERT_TRUE(p.HasMoreCommands());
        p.Advance();

        ASSERT_EQ(p.GetCommandType(), COMMAND::C_COMMAND);
        ASSERT_EQ(p.GetDest(), "D");
        ASSERT_EQ(p.GetComp(), "D+M");

        p.Advance();
        ASSERT_EQ(p.GetCommandType(), COMMAND::C_COMMAND);
        ASSERT_EQ(p.GetDest(), "AM");
        ASSERT_EQ(p.GetComp(), "!M");
        ASSERT_EQ(p.GetJump(), "JMP");

        p.Advance();
        ASSERT_EQ(p.GetCommandType(), COMMAND::C_COMMAND);
        ASSERT_EQ(p.GetComp(), "A-D");
        ASSERT_EQ(p.GetJump(), "JEQ");
    }
} // namespace compiler
