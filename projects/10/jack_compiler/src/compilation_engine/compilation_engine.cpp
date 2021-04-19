#include <memory>

#include "compilation_engine/compilation_engine.hpp"
#include "exception/exception.hpp"

namespace jack_compiler
{
    CompilationEngine::CompilationEngine(std::istream* input, std::ostream* output): tokenizer_(input) {
        writer_ = new XMLWriter(output);
    }
    
    CompilationEngine::CompilationEngine(const std::string& input_file, const std::string& output_file): tokenizer_(input_file) {
        writer_ = new XMLWriter(output_file);
    }

    CompilationEngine::~CompilationEngine() {
        delete writer_;
    }

    void CompilationEngine::MoveToNext() {
        if (!tokenizer_.HasMoreTokens()) {
            THROW_COMPILER_EXCEPT
        }
        tokenizer_.Advance();
    }

    void CompilationEngine::CompileClass() {
        Node::TokenUnion token_union;
        // class
        MoveToNext();
        if (tokenizer_.GetTokenType() != TERMINAL_TOKEN_TYPE::KEYWORD || tokenizer_.GetKeyword() != KEYWORD_TYPE::CLASS) {
            THROW_COMPILER_EXCEPT
        }
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::CLASS;
        root_ = std::make_shared<Node>(false, token_union);

        // class name
        MoveToNext();
        if (tokenizer_.GetTokenType() != TERMINAL_TOKEN_TYPE::IDENTIFIER) {
            THROW_COMPILER_EXCEPT
        }
        token_union.terminal_token_ = TERMINAL_TOKEN_TYPE::IDENTIFIER;
        auto node = std::make_shared<Node>(true, token_union, tokenizer_.GetIdentifier());
        root_->AppendChild(node);

        // { 
        MoveToNext();
        if (tokenizer_.GetTokenType() != TERMINAL_TOKEN_TYPE::SYMBOL || tokenizer_.GetSymbol() != "{") {
            THROW_COMPILER_EXCEPT
        }
        token_union.terminal_token_ = TERMINAL_TOKEN_TYPE::SYMBOL;
        node = std::make_shared<Node>(true, token_union, tokenizer_.GetSymbol());
        root_->AppendChild(node);

        while (true) {
            MoveToNext();
            if (tokenizer_.GetTokenType() == TERMINAL_TOKEN_TYPE::KEYWORD) {
                auto keyword = tokenizer_.GetKeyword();
                if (keyword == KEYWORD_TYPE::STATIC ||
                    keyword == KEYWORD_TYPE::FIELD) {
                    // class var dec
                    CompileClassVarDec(root_);
                } else if (keyword == KEYWORD_TYPE::CONSTRUCTOR ||
                    keyword == KEYWORD_TYPE::FUNCTION ||
                    keyword == KEYWORD_TYPE::METHOD) {
                    // class subroutine
                    CompileSubroutine();
                } else {
                    THROW_COMPILER_EXCEPT
                }
            } else if (tokenizer_.GetTokenType() == TERMINAL_TOKEN_TYPE::SYMBOL && tokenizer_.GetSymbol() == "}") {
                token_union.terminal_token_ = TERMINAL_TOKEN_TYPE::SYMBOL;
                node = std::make_shared<Node>(true, token_union, tokenizer_.GetSymbol());
                root_->AppendChild(node);
                break;
            } else {
                THROW_COMPILER_EXCEPT
            }
        }

        writer_->WriteToOutput(root_);
    };

    void CompilationEngine::CompileClassVarDec(std::shared_ptr<Node> parent) {
        Node::TokenUnion token_union;
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::CLASS_VAR_DEC;
        auto current = std::make_shared<Node>(false, token_union);
        parent->AppendChild(current);

        // static || field
        if (tokenizer_.GetTokenType() != TERMINAL_TOKEN_TYPE::KEYWORD ||
           !(tokenizer_.GetKeyword() == KEYWORD_TYPE::STATIC ||
           tokenizer_.GetKeyword() == KEYWORD_TYPE::FIELD)) {
            THROW_COMPILER_EXCEPT
        }
        token_union.terminal_token_ = TERMINAL_TOKEN_TYPE::KEYWORD;
        auto node = std::make_shared<Node>(true, token_union, KEYWORD_STR_TABLE.at(tokenizer_.GetKeyword()));
        current->AppendChild(node);
        
        // type: int, char, boolean, class_name 
        MoveToNext();
        if (tokenizer_.GetTokenType() == TERMINAL_TOKEN_TYPE::KEYWORD &&
           (tokenizer_.GetKeyword() == KEYWORD_TYPE::INT ||
           tokenizer_.GetKeyword() == KEYWORD_TYPE::CHAR ||
           tokenizer_.GetKeyword() == KEYWORD_TYPE::BOOLEAN)) {
            token_union.terminal_token_ = TERMINAL_TOKEN_TYPE::KEYWORD;
            node = std::make_shared<Node>(true, token_union, KEYWORD_STR_TABLE.at(tokenizer_.GetKeyword()));
            current->AppendChild(node);
        } else if (tokenizer_.GetTokenType() == TERMINAL_TOKEN_TYPE::IDENTIFIER) {
            token_union.terminal_token_ = TERMINAL_TOKEN_TYPE::IDENTIFIER;
            node = std::make_shared<Node>(true, token_union, tokenizer_.GetIdentifier());
            current->AppendChild(node);
        } else {
            THROW_COMPILER_EXCEPT
        }

        // var name
        MoveToNext();
        if (tokenizer_.GetTokenType() != TERMINAL_TOKEN_TYPE::IDENTIFIER) {
            THROW_COMPILER_EXCEPT
        }
        token_union.terminal_token_ = TERMINAL_TOKEN_TYPE::IDENTIFIER;
        node = std::make_shared<Node>(true, token_union, tokenizer_.GetIdentifier());
        current->AppendChild(node);

        // ',' var name
        MoveToNext();
        while (tokenizer_.GetTokenType() == TERMINAL_TOKEN_TYPE::SYMBOL && 
                tokenizer_.GetSymbol() == ",") {
            token_union.terminal_token_ = TERMINAL_TOKEN_TYPE::SYMBOL;
            node = std::make_shared<Node>(true, token_union, tokenizer_.GetSymbol());
            current->AppendChild(node);

            MoveToNext();
            if (tokenizer_.GetTokenType() != TERMINAL_TOKEN_TYPE::IDENTIFIER) {
                THROW_COMPILER_EXCEPT
            }
            token_union.terminal_token_ = TERMINAL_TOKEN_TYPE::IDENTIFIER;
            node = std::make_shared<Node>(true, token_union, tokenizer_.GetIdentifier());
            current->AppendChild(node);
            MoveToNext();
        }

        // ';'
        if (tokenizer_.GetTokenType() != TERMINAL_TOKEN_TYPE::SYMBOL || tokenizer_.GetSymbol() != ";") {
            THROW_COMPILER_EXCEPT
        }
        token_union.terminal_token_ = TERMINAL_TOKEN_TYPE::SYMBOL;
        node = std::make_shared<Node>(true, token_union, tokenizer_.GetSymbol());
        current->AppendChild(node);
    };

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
