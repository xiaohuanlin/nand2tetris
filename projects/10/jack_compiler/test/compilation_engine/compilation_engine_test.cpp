#include <iostream>
#include <fstream>
#include "gtest/gtest.h"

#include "compilation_engine/compilation_engine.hpp"
#include "exception/exception.hpp"

namespace jack_compiler
{
    TEST(CompilationEngineTest, ClassTest) {
        std::stringbuf osb("");
        std::ostream os(&osb);

        std::stringbuf isb("class Test\n{}");
        std::istream is(&isb);

        CompilationEngine engine(&is, &os);
        engine.CompileClass();

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size];
        buffer->sgetn(content, size);
        ASSERT_EQ("<class><identifier>Test</identifier><symbol>{</symbol><symbol>}</symbol></class>", std::string(content));
    }

    TEST(CompilationEngineTest, ClassVarDecTest) {
        std::stringbuf osb("");
        std::ostream os(&osb);

        std::stringbuf isb("class Test\n{ static int a,b;}");
        std::istream is(&isb);

        CompilationEngine engine(&is, &os);
        engine.CompileClass();

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size];
        buffer->sgetn(content, size);
        ASSERT_EQ("<class>"
                        "<identifier>Test</identifier>"
                        "<symbol>{</symbol>"
                        "<classVarDec>"
                            "<keyword>static</keyword>"
                            "<keyword>int</keyword>"
                            "<identifier>a</identifier>"
                            "<symbol>,</symbol>"
                            "<identifier>b</identifier>"
                            "<symbol>;</symbol>"
                        "</classVarDec>"
                        "<symbol>}</symbol>"
                    "</class>", std::string(content));
    }
}