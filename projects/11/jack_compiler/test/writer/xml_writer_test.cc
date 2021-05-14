
#include <iostream>
#include <fstream>
#include <memory>
#include "gtest/gtest.h"

#include "writer/vm/vm_writer.h"

namespace jack_compiler {
    TEST(VMWriterTest, Single) {
        std::stringbuf sb("");
        std::ostream os(&sb);
        VMWriter writer(&os);
        Node::TokenUnion token_type;
        token_type.terminal_token_ = TERMINAL_TOKEN_TYPE::IDENTIFIER;
        auto item = std::make_shared<Node>(true, token_type, "test");
        writer.WriteToOutput(item);

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size + 1];
        content[size] = '\0';
        buffer->sgetn(content, size);
        ASSERT_EQ("<identifier> test </identifier>\n", std::string(content));
    }
}  // namespace jack_compiler
