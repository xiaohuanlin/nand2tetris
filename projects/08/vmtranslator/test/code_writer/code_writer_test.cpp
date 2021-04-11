
#include <vector>
#include <string>

#include "gtest/gtest.h"

#include "code_writer/code_writer.hpp"

namespace vmtranslator
{
    TEST(CodeWriterTest, WriteArithmeticCommand) {
        std::stringbuf sb("");
        std::ostream output(&sb);

        CodeWriter code_writer(&output);
        std::string file_name = "./test.vm";
        code_writer.SetFileName(file_name);

        code_writer.WriteArithmetic("add");
        code_writer.Close();

        ASSERT_STREQ("@0\nA=M-1\nD=M\n@0\nM=M-1\nA=M-1\nM=M+D\n", sb.str().c_str());
    }
} // namespace vmtranslator
