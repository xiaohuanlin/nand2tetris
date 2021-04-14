#include "compilation_engine/compilation_engine.hpp"

namespace jack_compiler
{
    CompilationEngine::CompilationEngine(std::istream* input, std::ostream* output): input_(input), output_(output) {
    }
    
    CompilationEngine::CompilationEngine(const std::string& input_file, const std::string& output_file) {
        input_ = new std::ifstream(input_file);
        output_ = new std::ofstream(output_file);
    }

    CompilationEngine::~CompilationEngine() {
        delete input_;
        delete output_;
    }
    void CompilationEngine::CompileClass() {};
    void CompilationEngine::CompileClassVarDec() {};
    void CompilationEngine::CompileSubroutine() {};
    void CompilationEngine::CompileParameterList() {};
    void CompilationEngine::CompileVarDec() {};
    void CompilationEngine::CompileStatement() {};
    void CompilationEngine::CompileDo() {};
    void CompilationEngine::CompileLet() {};
    void CompilationEngine::CompileWhile() {};
    void CompilationEngine::CompileReturn() {};
    void CompilationEngine::CompileIf() {};
    void CompilationEngine::CompileExpression() {};
    void CompilationEngine::CompileTerm() {};
    void CompilationEngine::CompileExpressionList() {};
    
} // namespace jack_compiler
