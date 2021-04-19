
#include <iostream>
#include <fstream>
#include <memory>
#include "gtest/gtest.h"

#include "writer/xml/xml_writer.hpp"

namespace jack_compiler
{
    TEST(XMLWriterTest, Single) {
        std::stringbuf sb("");
        std::ostream os(&sb);
        XMLWriter writer(&os);
        Node::TokenUnion token_type;
        token_type.terminal_token_ = TERMINAL_TOKEN_TYPE::IDENTIFIER;
        auto item = std::make_shared<Node>(true, token_type, "test");
        writer.WriteToOutput(item);

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size];
        buffer->sgetn(content, size);
        ASSERT_EQ("<identifier>test</identifier>", std::string(content));
    }

    TEST(XMLWriterTest, LevelTwo) {
        std::stringbuf sb("");
        std::ostream os(&sb);
        XMLWriter writer(&os);

        Node::TokenUnion token_type;
        token_type.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::CLASS;
        auto item_root = std::make_shared<Node>(false, token_type);
        token_type.terminal_token_ = TERMINAL_TOKEN_TYPE::SYMBOL;
        auto item_child1 = std::make_shared<Node>(true, token_type, "{");
        auto item_child2 = std::make_shared<Node>(true, token_type, "}");
        item_root->AppendChild(std::move(item_child1));
        item_root->AppendChild(std::move(item_child2));
        writer.WriteToOutput(item_root);

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size];
        buffer->sgetn(content, size);
        ASSERT_EQ("<class><symbol>{</symbol><symbol>}</symbol></class>", std::string(content));
    }

    TEST(XMLWriterTest, LevelThree) {
        std::stringbuf sb("");
        std::ostream os(&sb);
        XMLWriter writer(&os);

        Node::TokenUnion token_type;
        token_type.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::CLASS;
        auto item_root = std::make_shared<Node>(false, token_type);

        token_type.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::STATEMENTS;
        auto item_child1 = std::make_shared<Node>(false, token_type);

        token_type.terminal_token_ = TERMINAL_TOKEN_TYPE::SYMBOL;
        auto item_child2 = std::make_shared<Node>(true, token_type, ";");

        token_type.terminal_token_ = TERMINAL_TOKEN_TYPE::KEYWORD;
        auto item_grandchild1 = std::make_shared<Node>(true, token_type, "return");

        token_type.terminal_token_ = TERMINAL_TOKEN_TYPE::IDENTIFIER;
        auto item_grandchild2 = std::make_shared<Node>(true, token_type, "x");

        item_child1->AppendChild(std::move(item_grandchild1));
        item_child1->AppendChild(std::move(item_grandchild2));
        item_root->AppendChild(std::move(item_child1));
        item_root->AppendChild(std::move(item_child2));
        writer.WriteToOutput(item_root);

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size];
        buffer->sgetn(content, size);
        ASSERT_EQ(
            "<class><statements><keyword>return</keyword><identifier>x</identifier></statements><symbol>;</symbol></class>",
            std::string(content)
        );
    }
}