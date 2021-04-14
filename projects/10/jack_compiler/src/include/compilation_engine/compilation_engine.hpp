#pragma once
#include <iostream>
#include <fstream>

namespace jack_compiler {
    class CompilationEngine {
    public:
        explicit CompilationEngine(std::istream* input, std::ostream* output);
        CompilationEngine(const std::string& input_file, const std::string& output_file);
        ~CompilationEngine();

        /**
         * Compile whole class
         */
        void CompileClass();
        /**
         * Compile static or field declaration
         */
        void CompileClassVarDec();
        void CompileSubroutine();
        void CompileParameterList();
        void CompileVarDec();
        /**
         * Compile statement except for "{}"
         */
        void CompileStatement();
        void CompileDo();
        void CompileLet();
        void CompileWhile();
        void CompileReturn();
        void CompileIf();
        void CompileExpression();
        /**
         * We should peek next word to distinguish among the variable, the array and the subroution call
         */
        void CompileTerm();
        void CompileExpressionList();
    private:
        std::istream* input_;
        std::ostream* output_;
    };
} // namespace jack_compiler
