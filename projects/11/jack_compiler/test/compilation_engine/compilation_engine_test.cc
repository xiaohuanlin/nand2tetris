#include <iostream>
#include <fstream>
#include "gtest/gtest.h"

#include "compilation_engine/compilation_engine.h"
#include "exception/exception.h"
#include "writer/xml/xml_writer.h"

namespace jack_compiler {
    TEST(CompilationEngineTest, ClassTest) {
        std::stringbuf osb("");
        std::ostream os(&osb);

        std::stringbuf isb("class Test\n{}");
        std::istream is(&isb);

        CompilationEngine<XMLWriter> engine(&is, &os);
        engine.CompileClass();

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size + 1];
        buffer->sgetn(content, size);
        content[size] = '\0';
        ASSERT_EQ("<class>\n"
                        "\t<keyword> class </keyword>\n"
                        "\t<identifier> Test </identifier>\n"
                        "\t<symbol> { </symbol>\n"
                        "\t<symbol> } </symbol>\n"
                    "</class>\n", std::string(content));
    }

    TEST(CompilationEngineTest, ClassVarDecTest) {
        std::stringbuf osb("");
        std::ostream os(&osb);

        std::stringbuf isb("class Test\n{ static int a,b;}");
        std::istream is(&isb);

        CompilationEngine<XMLWriter> engine(&is, &os);
        engine.CompileClass();

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size + 1];
        buffer->sgetn(content, size);
        content[size] = '\0';
        ASSERT_EQ("<class>\n"
                        "\t<keyword> class </keyword>\n"
                        "\t<identifier> Test </identifier>\n"
                        "\t<symbol> { </symbol>\n"
                        "\t<classVarDec>\n"
                            "\t\t<keyword> static </keyword>\n"
                            "\t\t<keyword> int </keyword>\n"
                            "\t\t<identifier> a </identifier>\n"
                            "\t\t<symbol> , </symbol>\n"
                            "\t\t<identifier> b </identifier>\n"
                            "\t\t<symbol> ; </symbol>\n"
                        "\t</classVarDec>\n"
                        "\t<symbol> } </symbol>\n"
                    "</class>\n", std::string(content));
    }

    TEST(CompilationEngineTest, SubroutineTest) {
        std::stringbuf osb("");
        std::ostream os(&osb);

        std::stringbuf isb("class Test\n{ function void test() {}}");
        std::istream is(&isb);

        CompilationEngine<XMLWriter> engine(&is, &os);
        engine.CompileClass();

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size + 1];
        buffer->sgetn(content, size);
        content[size] = '\0';
        ASSERT_EQ("<class>\n"
                        "\t<keyword> class </keyword>\n"
                        "\t<identifier> Test </identifier>\n"
                        "\t<symbol> { </symbol>\n"
                        "\t<subroutineDec>\n"
                            "\t\t<keyword> function </keyword>\n"
                            "\t\t<keyword> void </keyword>\n"
                            "\t\t<identifier> test </identifier>\n"
                            "\t\t<symbol> ( </symbol>\n"
                            "\t\t<parameterList>\n"
                            "\t\t</parameterList>\n"
                            "\t\t<symbol> ) </symbol>\n"
                            "\t\t<subroutineBody>\n"
                                "\t\t\t<symbol> { </symbol>\n"
                                "\t\t\t<statements>\n"
                                "\t\t\t</statements>\n"
                                "\t\t\t<symbol> } </symbol>\n"
                            "\t\t</subroutineBody>\n"
                        "\t</subroutineDec>\n"
                        "\t<symbol> } </symbol>\n"
                    "</class>\n", std::string(content));
    }

    TEST(CompilationEngineTest, SubroutineParameterTest) {
        std::stringbuf osb("");
        std::ostream os(&osb);

        std::stringbuf isb("class Test\n{ function void test(int a, boolean b) {}}");
        std::istream is(&isb);

        CompilationEngine<XMLWriter> engine(&is, &os);
        engine.CompileClass();

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size + 1];
        buffer->sgetn(content, size);
        content[size] = '\0';
        ASSERT_EQ("<class>\n"
                        "\t<keyword> class </keyword>\n"
                        "\t<identifier> Test </identifier>\n"
                        "\t<symbol> { </symbol>\n"
                        "\t<subroutineDec>\n"
                            "\t\t<keyword> function </keyword>\n"
                            "\t\t<keyword> void </keyword>\n"
                            "\t\t<identifier> test </identifier>\n"
                            "\t\t<symbol> ( </symbol>\n"
                            "\t\t<parameterList>\n"
                                "\t\t\t<keyword> int </keyword>\n"
                                "\t\t\t<identifier> a </identifier>\n"
                                "\t\t\t<symbol> , </symbol>\n"
                                "\t\t\t<keyword> boolean </keyword>\n"
                                "\t\t\t<identifier> b </identifier>\n"
                            "\t\t</parameterList>\n"
                            "\t\t<symbol> ) </symbol>\n"
                            "\t\t<subroutineBody>\n"
                                "\t\t\t<symbol> { </symbol>\n"
                                "\t\t\t<statements>\n"
                                "\t\t\t</statements>\n"
                                "\t\t\t<symbol> } </symbol>\n"
                            "\t\t</subroutineBody>\n"
                        "\t</subroutineDec>\n"
                        "\t<symbol> } </symbol>\n"
                    "</class>\n", std::string(content));
    }

    TEST(CompilationEngineTest, SubroutineBodyVarDecTest) {
        std::stringbuf osb("");
        std::ostream os(&osb);

        std::stringbuf isb("class Test\n{ function void test(int a, boolean b) { var int c,d; var char e;}}");
        std::istream is(&isb);

        CompilationEngine<XMLWriter> engine(&is, &os);
        engine.CompileClass();

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size + 1];
        buffer->sgetn(content, size);
        content[size] = '\0';
        ASSERT_EQ("<class>\n"
                        "\t<keyword> class </keyword>\n"
                        "\t<identifier> Test </identifier>\n"
                        "\t<symbol> { </symbol>\n"
                        "\t<subroutineDec>\n"
                            "\t\t<keyword> function </keyword>\n"
                            "\t\t<keyword> void </keyword>\n"
                            "\t\t<identifier> test </identifier>\n"
                            "\t\t<symbol> ( </symbol>\n"
                            "\t\t<parameterList>\n"
                                "\t\t\t<keyword> int </keyword>\n"
                                "\t\t\t<identifier> a </identifier>\n"
                                "\t\t\t<symbol> , </symbol>\n"
                                "\t\t\t<keyword> boolean </keyword>\n"
                                "\t\t\t<identifier> b </identifier>\n"
                            "\t\t</parameterList>\n"
                            "\t\t<symbol> ) </symbol>\n"
                            "\t\t<subroutineBody>\n"
                                "\t\t\t<symbol> { </symbol>\n"
                                "\t\t\t<varDec>\n"
                                    "\t\t\t\t<keyword> var </keyword>\n"
                                    "\t\t\t\t<keyword> int </keyword>\n"
                                    "\t\t\t\t<identifier> c </identifier>\n"
                                    "\t\t\t\t<symbol> , </symbol>\n"
                                    "\t\t\t\t<identifier> d </identifier>\n"
                                    "\t\t\t\t<symbol> ; </symbol>\n"
                                "\t\t\t</varDec>\n"
                                "\t\t\t<varDec>\n"
                                    "\t\t\t\t<keyword> var </keyword>\n"
                                    "\t\t\t\t<keyword> char </keyword>\n"
                                    "\t\t\t\t<identifier> e </identifier>\n"
                                    "\t\t\t\t<symbol> ; </symbol>\n"
                                "\t\t\t</varDec>\n"
                                "\t\t\t<statements>\n"
                                "\t\t\t</statements>\n"
                                "\t\t\t<symbol> } </symbol>\n"
                            "\t\t</subroutineBody>\n"
                        "\t</subroutineDec>\n"
                        "\t<symbol> } </symbol>\n"
                    "</class>\n", std::string(content));
    }

    TEST(CompilationEngineTest, SubroutineBodyLetStatementTest) {
        std::stringbuf osb("");
        std::ostream os(&osb);

        std::stringbuf isb("class Test\n{ function void test() {var Array a; let a[0] = 3;}}");
        std::istream is(&isb);

        CompilationEngine<XMLWriter> engine(&is, &os);
        engine.CompileClass();

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size + 1];
        buffer->sgetn(content, size);
        content[size] = '\0';
        ASSERT_EQ("<class>\n"
                        "\t<keyword> class </keyword>\n"
                        "\t<identifier> Test </identifier>\n"
                        "\t<symbol> { </symbol>\n"
                        "\t<subroutineDec>\n"
                            "\t\t<keyword> function </keyword>\n"
                            "\t\t<keyword> void </keyword>\n"
                            "\t\t<identifier> test </identifier>\n"
                            "\t\t<symbol> ( </symbol>\n"
                            "\t\t<parameterList>\n"
                            "\t\t</parameterList>\n"
                            "\t\t<symbol> ) </symbol>\n"
                            "\t\t<subroutineBody>\n"
                                "\t\t\t<symbol> { </symbol>\n"
                                "\t\t\t<varDec>\n"
                                    "\t\t\t\t<keyword> var </keyword>\n"
                                    "\t\t\t\t<identifier> Array </identifier>\n"
                                    "\t\t\t\t<identifier> a </identifier>\n"
                                    "\t\t\t\t<symbol> ; </symbol>\n"
                                "\t\t\t</varDec>\n"
                                "\t\t\t<statements>\n"
                                    "\t\t\t\t<letStatement>\n"
                                        "\t\t\t\t\t<keyword> let </keyword>\n"
                                        "\t\t\t\t\t<identifier> a </identifier>\n"
                                        "\t\t\t\t\t<symbol> [ </symbol>\n"
                                        "\t\t\t\t\t<expression>\n"
                                            "\t\t\t\t\t\t<term>\n"
                                                "\t\t\t\t\t\t\t<integerConstant> 0 </integerConstant>\n"
                                            "\t\t\t\t\t\t</term>\n"
                                        "\t\t\t\t\t</expression>\n"
                                        "\t\t\t\t\t<symbol> ] </symbol>\n"
                                        "\t\t\t\t\t<symbol> = </symbol>\n"
                                        "\t\t\t\t\t<expression>\n"
                                            "\t\t\t\t\t\t<term>\n"
                                                "\t\t\t\t\t\t\t<integerConstant> 3 </integerConstant>\n"
                                            "\t\t\t\t\t\t</term>\n"
                                        "\t\t\t\t\t</expression>\n"
                                        "\t\t\t\t\t<symbol> ; </symbol>\n"
                                    "\t\t\t\t</letStatement>\n"
                                "\t\t\t</statements>\n"
                                "\t\t\t<symbol> } </symbol>\n"
                            "\t\t</subroutineBody>\n"
                        "\t</subroutineDec>\n"
                        "\t<symbol> } </symbol>\n"
                    "</class>\n", std::string(content));
    }

    TEST(CompilationEngineTest, SubroutineBodyWhileStatementTest) {
        std::stringbuf osb("");
        std::ostream os(&osb);

        std::stringbuf isb("class Test\n{ function void test() {while (1-false) {}}}");
        std::istream is(&isb);

        CompilationEngine<XMLWriter> engine(&is, &os);
        engine.CompileClass();

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size + 1];
        buffer->sgetn(content, size);
        content[size] = '\0';
        ASSERT_EQ("<class>\n"
                        "\t<keyword> class </keyword>\n"
                        "\t<identifier> Test </identifier>\n"
                        "\t<symbol> { </symbol>\n"
                        "\t<subroutineDec>\n"
                            "\t\t<keyword> function </keyword>\n"
                            "\t\t<keyword> void </keyword>\n"
                            "\t\t<identifier> test </identifier>\n"
                            "\t\t<symbol> ( </symbol>\n"
                            "\t\t<parameterList>\n"
                            "\t\t</parameterList>\n"
                            "\t\t<symbol> ) </symbol>\n"
                            "\t\t<subroutineBody>\n"
                                "\t\t\t<symbol> { </symbol>\n"
                                "\t\t\t<statements>\n"
                                    "\t\t\t\t<whileStatement>\n"
                                        "\t\t\t\t\t<keyword> while </keyword>\n"
                                        "\t\t\t\t\t<symbol> ( </symbol>\n"
                                        "\t\t\t\t\t<expression>\n"
                                            "\t\t\t\t\t\t<term>\n"
                                                "\t\t\t\t\t\t\t<integerConstant> 1 </integerConstant>\n"
                                            "\t\t\t\t\t\t</term>\n"
                                            "\t\t\t\t\t\t<symbol> - </symbol>\n"
                                            "\t\t\t\t\t\t<term>\n"
                                                "\t\t\t\t\t\t\t<keyword> false </keyword>\n"
                                            "\t\t\t\t\t\t</term>\n"
                                        "\t\t\t\t\t</expression>\n"
                                        "\t\t\t\t\t<symbol> ) </symbol>\n"
                                        "\t\t\t\t\t<symbol> { </symbol>\n"
                                        "\t\t\t\t\t<statements>\n"
                                        "\t\t\t\t\t</statements>\n"
                                        "\t\t\t\t\t<symbol> } </symbol>\n"
                                    "\t\t\t\t</whileStatement>\n"
                                "\t\t\t</statements>\n"
                                "\t\t\t<symbol> } </symbol>\n"
                            "\t\t</subroutineBody>\n"
                        "\t</subroutineDec>\n"
                        "\t<symbol> } </symbol>\n"
                    "</class>\n", std::string(content));
    }

    TEST(CompilationEngineTest, SubroutineBodyReturnStatementTest) {
        std::stringbuf osb("");
        std::ostream os(&osb);

        std::stringbuf isb("class Test\n{ function void test() {return;}}");
        std::istream is(&isb);

        CompilationEngine<XMLWriter> engine(&is, &os);
        engine.CompileClass();

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size + 1];
        buffer->sgetn(content, size);
        content[size] = '\0';
        ASSERT_EQ("<class>\n"
                        "\t<keyword> class </keyword>\n"
                        "\t<identifier> Test </identifier>\n"
                        "\t<symbol> { </symbol>\n"
                        "\t<subroutineDec>\n"
                            "\t\t<keyword> function </keyword>\n"
                            "\t\t<keyword> void </keyword>\n"
                            "\t\t<identifier> test </identifier>\n"
                            "\t\t<symbol> ( </symbol>\n"
                            "\t\t<parameterList>\n"
                            "\t\t</parameterList>\n"
                            "\t\t<symbol> ) </symbol>\n"
                            "\t\t<subroutineBody>\n"
                                "\t\t\t<symbol> { </symbol>\n"
                                "\t\t\t<statements>\n"
                                    "\t\t\t\t<returnStatement>\n"
                                        "\t\t\t\t\t<keyword> return </keyword>\n"
                                        "\t\t\t\t\t<symbol> ; </symbol>\n"
                                    "\t\t\t\t</returnStatement>\n"
                                "\t\t\t</statements>\n"
                                "\t\t\t<symbol> } </symbol>\n"
                            "\t\t</subroutineBody>\n"
                        "\t</subroutineDec>\n"
                        "\t<symbol> } </symbol>\n"
                    "</class>\n", std::string(content));
    }

    TEST(CompilationEngineTest, SubroutineBodyIfStatementTest) {
        std::stringbuf osb("");
        std::ostream os(&osb);

        std::stringbuf isb("class Test\n{ function void test() {if ((~1+2) > 5) {} else {}}}");
        std::istream is(&isb);

        CompilationEngine<XMLWriter> engine(&is, &os);
        engine.CompileClass();

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size + 1];
        buffer->sgetn(content, size);
        content[size] = '\0';
        ASSERT_EQ("<class>\n"
                        "\t<keyword> class </keyword>\n"
                        "\t<identifier> Test </identifier>\n"
                        "\t<symbol> { </symbol>\n"
                        "\t<subroutineDec>\n"
                            "\t\t<keyword> function </keyword>\n"
                            "\t\t<keyword> void </keyword>\n"
                            "\t\t<identifier> test </identifier>\n"
                            "\t\t<symbol> ( </symbol>\n"
                            "\t\t<parameterList>\n"
                            "\t\t</parameterList>\n"
                            "\t\t<symbol> ) </symbol>\n"
                            "\t\t<subroutineBody>\n"
                                "\t\t\t<symbol> { </symbol>\n"
                                "\t\t\t<statements>\n"
                                    "\t\t\t\t<ifStatement>\n"
                                        "\t\t\t\t\t<keyword> if </keyword>\n"
                                        "\t\t\t\t\t<symbol> ( </symbol>\n"
                                        "\t\t\t\t\t<expression>\n"
                                            "\t\t\t\t\t\t<term>\n"
                                                "\t\t\t\t\t\t\t<symbol> ( </symbol>\n"
                                                "\t\t\t\t\t\t\t<expression>\n"
                                                    "\t\t\t\t\t\t\t\t<term>\n"
                                                        "\t\t\t\t\t\t\t\t\t<symbol> ~ </symbol>\n"
                                                        "\t\t\t\t\t\t\t\t\t<term>\n"
                                                            "\t\t\t\t\t\t\t\t\t\t<integerConstant> 1 "
                                                                                "</integerConstant>\n"
                                                        "\t\t\t\t\t\t\t\t\t</term>\n"
                                                    "\t\t\t\t\t\t\t\t</term>\n"
                                                    "\t\t\t\t\t\t\t\t<symbol> + </symbol>\n"
                                                    "\t\t\t\t\t\t\t\t<term>\n"
                                                        "\t\t\t\t\t\t\t\t\t<integerConstant> 2 </integerConstant>\n"
                                                    "\t\t\t\t\t\t\t\t</term>\n"
                                                "\t\t\t\t\t\t\t</expression>\n"
                                                "\t\t\t\t\t\t\t<symbol> ) </symbol>\n"
                                            "\t\t\t\t\t\t</term>\n"
                                            "\t\t\t\t\t\t<symbol> &gt; </symbol>\n"
                                            "\t\t\t\t\t\t<term>\n"
                                                "\t\t\t\t\t\t\t<integerConstant> 5 </integerConstant>\n"
                                            "\t\t\t\t\t\t</term>\n"
                                        "\t\t\t\t\t</expression>\n"
                                        "\t\t\t\t\t<symbol> ) </symbol>\n"
                                        "\t\t\t\t\t<symbol> { </symbol>\n"
                                        "\t\t\t\t\t<statements>\n"
                                        "\t\t\t\t\t</statements>\n"
                                        "\t\t\t\t\t<symbol> } </symbol>\n"
                                        "\t\t\t\t\t<keyword> else </keyword>\n"
                                        "\t\t\t\t\t<symbol> { </symbol>\n"
                                        "\t\t\t\t\t<statements>\n"
                                        "\t\t\t\t\t</statements>\n"
                                        "\t\t\t\t\t<symbol> } </symbol>\n"
                                    "\t\t\t\t</ifStatement>\n"
                                "\t\t\t</statements>\n"
                                "\t\t\t<symbol> } </symbol>\n"
                            "\t\t</subroutineBody>\n"
                        "\t</subroutineDec>\n"
                        "\t<symbol> } </symbol>\n"
                    "</class>\n", std::string(content));
    }

    TEST(CompilationEngineTest, SubroutineBodyDoStatementTest) {
        std::stringbuf osb("");
        std::ostream os(&osb);

        std::stringbuf isb("class Test\n{ function void test() {do Screen.test(3 + 5);}}");
        std::istream is(&isb);

        CompilationEngine<XMLWriter> engine(&is, &os);
        engine.CompileClass();

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size + 1];
        buffer->sgetn(content, size);
        content[size] = '\0';
        ASSERT_EQ("<class>\n"
                        "\t<keyword> class </keyword>\n"
                        "\t<identifier> Test </identifier>\n"
                        "\t<symbol> { </symbol>\n"
                        "\t<subroutineDec>\n"
                            "\t\t<keyword> function </keyword>\n"
                            "\t\t<keyword> void </keyword>\n"
                            "\t\t<identifier> test </identifier>\n"
                            "\t\t<symbol> ( </symbol>\n"
                            "\t\t<parameterList>\n"
                            "\t\t</parameterList>\n"
                            "\t\t<symbol> ) </symbol>\n"
                            "\t\t<subroutineBody>\n"
                                "\t\t\t<symbol> { </symbol>\n"
                                "\t\t\t<statements>\n"
                                    "\t\t\t\t<doStatement>\n"
                                        "\t\t\t\t\t<keyword> do </keyword>\n"
                                        "\t\t\t\t\t<identifier> Screen </identifier>\n"
                                        "\t\t\t\t\t<symbol> . </symbol>\n"
                                        "\t\t\t\t\t<identifier> test </identifier>\n"
                                        "\t\t\t\t\t<symbol> ( </symbol>\n"
                                        "\t\t\t\t\t<expressionList>\n"
                                            "\t\t\t\t\t\t<expression>\n"
                                                "\t\t\t\t\t\t\t<term>\n"
                                                    "\t\t\t\t\t\t\t\t<integerConstant> 3 </integerConstant>\n"
                                                "\t\t\t\t\t\t\t</term>\n"
                                                "\t\t\t\t\t\t\t<symbol> + </symbol>\n"
                                                "\t\t\t\t\t\t\t<term>\n"
                                                    "\t\t\t\t\t\t\t\t<integerConstant> 5 </integerConstant>\n"
                                                "\t\t\t\t\t\t\t</term>\n"
                                            "\t\t\t\t\t\t</expression>\n"
                                        "\t\t\t\t\t</expressionList>\n"
                                        "\t\t\t\t\t<symbol> ) </symbol>\n"
                                        "\t\t\t\t\t<symbol> ; </symbol>\n"
                                    "\t\t\t\t</doStatement>\n"
                                "\t\t\t</statements>\n"
                                "\t\t\t<symbol> } </symbol>\n"
                            "\t\t</subroutineBody>\n"
                        "\t</subroutineDec>\n"
                        "\t<symbol> } </symbol>\n"
                    "</class>\n", std::string(content));
    }

    TEST(CompilationEngineTest, SubroutineCall) {
        std::stringbuf osb("");
        std::ostream os(&osb);

        std::stringbuf isb("class Test\n{ function void test() {do Mock(x, y, z);}}");
        std::istream is(&isb);

        CompilationEngine<XMLWriter> engine(&is, &os);
        engine.CompileClass();

        auto buffer = os.rdbuf();
        std::streamsize size = buffer->pubseekoff(0, os.end);
        buffer->pubseekoff(0, os.beg);
        char content[size + 1];
        buffer->sgetn(content, size);
        content[size] = '\0';
        ASSERT_EQ("<class>\n"
                        "\t<keyword> class </keyword>\n"
                        "\t<identifier> Test </identifier>\n"
                        "\t<symbol> { </symbol>\n"
                        "\t<subroutineDec>\n"
                            "\t\t<keyword> function </keyword>\n"
                            "\t\t<keyword> void </keyword>\n"
                            "\t\t<identifier> test </identifier>\n"
                            "\t\t<symbol> ( </symbol>\n"
                            "\t\t<parameterList>\n"
                            "\t\t</parameterList>\n"
                            "\t\t<symbol> ) </symbol>\n"
                            "\t\t<subroutineBody>\n"
                                "\t\t\t<symbol> { </symbol>\n"
                                "\t\t\t<statements>\n"
                                    "\t\t\t\t<doStatement>\n"
                                        "\t\t\t\t\t<keyword> do </keyword>\n"
                                        "\t\t\t\t\t<identifier> Mock </identifier>\n"
                                        "\t\t\t\t\t<symbol> ( </symbol>\n"
                                        "\t\t\t\t\t<expressionList>\n"
                                            "\t\t\t\t\t\t<expression>\n"
                                                "\t\t\t\t\t\t\t<term>\n"
                                                    "\t\t\t\t\t\t\t\t<identifier> x </identifier>\n"
                                                "\t\t\t\t\t\t\t</term>\n"
                                            "\t\t\t\t\t\t</expression>\n"
                                            "\t\t\t\t\t\t<symbol> , </symbol>\n"
                                            "\t\t\t\t\t\t<expression>\n"
                                                "\t\t\t\t\t\t\t<term>\n"
                                                    "\t\t\t\t\t\t\t\t<identifier> y </identifier>\n"
                                                "\t\t\t\t\t\t\t</term>\n"
                                            "\t\t\t\t\t\t</expression>\n"
                                            "\t\t\t\t\t\t<symbol> , </symbol>\n"
                                            "\t\t\t\t\t\t<expression>\n"
                                                "\t\t\t\t\t\t\t<term>\n"
                                                    "\t\t\t\t\t\t\t\t<identifier> z </identifier>\n"
                                                "\t\t\t\t\t\t\t</term>\n"
                                            "\t\t\t\t\t\t</expression>\n"
                                        "\t\t\t\t\t</expressionList>\n"
                                        "\t\t\t\t\t<symbol> ) </symbol>\n"
                                        "\t\t\t\t\t<symbol> ; </symbol>\n"
                                    "\t\t\t\t</doStatement>\n"
                                "\t\t\t</statements>\n"
                                "\t\t\t<symbol> } </symbol>\n"
                            "\t\t</subroutineBody>\n"
                        "\t</subroutineDec>\n"
                        "\t<symbol> } </symbol>\n"
                    "</class>\n", std::string(content));
    }
}  // namespace jack_compiler
