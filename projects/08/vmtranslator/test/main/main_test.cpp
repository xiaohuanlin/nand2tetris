#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "gtest/gtest.h"

#include "main/main.hpp"

namespace vmtranslator 
{
    void RunTestByCase(char* category, char* directory, bool write_init = false) {
        Main m(write_init); 

        char arg1[] = "./main";

        char arg2[256];
        char template_str[] = "../../../08/%s/%s";
        sprintf(arg2, template_str, category, directory);

        char arg3[] = "./test.asm";

        char* commands[] {arg1, arg2, arg3};
        m.Run(3, commands);
        
        char result_template_str[] = "../../../08/%s/%s/%s.asm";
        sprintf(arg2, result_template_str, category, directory, directory);
        std::ifstream result_file(arg2);
        result_file.seekg(0, result_file.end);
        int length = result_file.tellg();
        result_file.seekg(0, result_file.beg);

        char result_buf[length + 1];
        result_buf[length] = '\0';
        result_file.read(result_buf, length);

        // test output
        std::ifstream test_file(arg3);
        test_file.seekg(0, test_file.end);
        length = test_file.tellg();
        test_file.seekg(0, test_file.beg);

        char test_buf[length + 1];
        test_buf[length] = '\0';
        test_file.read(test_buf, length);

        ASSERT_STREQ(result_buf, test_buf);
        unlink(arg3);
    }

    TEST(MainTest, BasicLoop) {
        char category[] = "ProgramFlow";
        char directory[] = "BasicLoop";
        RunTestByCase(category, directory);
    }
    
    TEST(MainTest, FibonacciSeries) {
        char category[] = "ProgramFlow";
        char directory[] = "FibonacciSeries";
        RunTestByCase(category, directory);
    }

    TEST(MainTest, SimpleFunction) {
        char category[] = "FunctionCalls";
        char directory[] = "SimpleFunction";
        RunTestByCase(category, directory);
    }

    TEST(MainTest, NestedCall) {
        char category[] = "FunctionCalls";
        char directory[] = "NestedCall";
        RunTestByCase(category, directory, true);
    }

    TEST(MainTest, FibonacciElement) {
        char category[] = "FunctionCalls";
        char directory[] = "FibonacciElement";
        RunTestByCase(category, directory, true);
    }
} // namespace vmtranslator 
