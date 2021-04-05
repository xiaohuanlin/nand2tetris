#include <string>

#include "gtest/gtest.h"

#include "parser.hpp"

namespace vmtranslator
{
    TEST(ParserTest, ArithmeticCommand) {
        std::stringbuf sb("add//comment\r\n  axdd  \r\nneg\r\n// comment\r\nand");
        std::istream input(&sb);
        Parser parser(&input);

        parser.Advance();
        ASSERT_EQ(COMMAND::C_ARITHMETIC, parser.GetCommandType());
        ASSERT_EQ("add", parser.GetArg1());

        parser.Advance();
        ASSERT_EQ(COMMAND::C_ARITHMETIC, parser.GetCommandType());
        ASSERT_EQ("neg", parser.GetArg1());

        parser.Advance();
        ASSERT_EQ(COMMAND::C_ARITHMETIC, parser.GetCommandType());
        ASSERT_EQ("and", parser.GetArg1());

        ASSERT_FALSE(parser.HasMoreCommands());
    }

    TEST(ParserTest, MemoryAccessConstantCommand) {
        std::stringbuf sb("push    constant    1\r\n push constant 2");
        std::istream input(&sb);
        Parser parser(&input);

        parser.Advance();
        ASSERT_EQ(COMMAND::C_PUSH, parser.GetCommandType());
        ASSERT_EQ("constant", parser.GetArg1());
        ASSERT_EQ(1, parser.GetArg2());

        parser.Advance();
        ASSERT_EQ(COMMAND::C_PUSH, parser.GetCommandType());
        ASSERT_EQ("constant", parser.GetArg1());
        ASSERT_EQ(2, parser.GetArg2());

        ASSERT_FALSE(parser.HasMoreCommands());
    }
} // namespace compiler
