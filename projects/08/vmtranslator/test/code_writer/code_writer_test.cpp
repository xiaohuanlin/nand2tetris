
#include <string>

#include "gtest/gtest.h"

#include "code_writer.hpp"

namespace vmtranslator
{
    TEST(CodeWriterTest, WriteArithmeticCommand) {
        std::stringbuf sb("");
        std::ostream output(&sb);

        CodeWriter code_writer(&output);
        code_writer.WriteArithmetic("add");

        std::string file_name = "./test.txt";
        code_writer.SetFileName(file_name);
        code_writer.Close();

        std::ifstream file(file_name);
        EXPECT_TRUE(file.is_open());
        std::streambuf *buf = file.rdbuf();
        std::streamsize size = buf->pubseekoff(0, file.end);
        buf->pubseekoff(0, file.beg);
        char* contents = new char [size + 1];
        contents[size] = '\0';
        buf->sgetn(contents, size);

        ASSERT_STREQ("@0\nA=M-1\nD=M\n@0\nM=M-1\nA=M-1\nM=M+D\n", contents);
        file.close();
        unlink(file_name.c_str());
    }
} // namespace compiler
