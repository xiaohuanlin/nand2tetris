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
} // namespace compiler
