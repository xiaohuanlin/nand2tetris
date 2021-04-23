#include <algorithm>
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
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::CLASS;
        root_ = std::make_shared<Node>(false, token_union);

        // class
        MoveToNext();
        if (!CompileKeyword(root_, {KEYWORD_TYPE::CLASS})) {
            THROW_COMPILER_EXCEPT
        }

        // class name
        MoveToNext();
        if (!CompileIdentifier(root_)) {
            THROW_COMPILER_EXCEPT
        }

        // { 
        MoveToNext();
        if (!CompileSymbol(root_, {"{"})) {
            THROW_COMPILER_EXCEPT
        }

        // class var dec
        MoveToNext();
        while (CompileKeyword(nullptr, {KEYWORD_TYPE::FIELD, KEYWORD_TYPE::STATIC})) {
            CompileClassVarDec(root_);
            MoveToNext();
        }

        // class subroutine
        while (CompileKeyword(nullptr, {KEYWORD_TYPE::CONSTRUCTOR, KEYWORD_TYPE::FUNCTION, KEYWORD_TYPE::METHOD})) {
            CompileSubroutine(root_);
            MoveToNext();
        }

        // } 
        if (!CompileSymbol(root_, {"}"})) {
            THROW_COMPILER_EXCEPT
        }
        writer_->WriteToOutput(root_);
    };

    void CompilationEngine::CompileClassVarDec(std::shared_ptr<Node> parent) {
        Node::TokenUnion token_union;
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::CLASS_VAR_DEC;
        auto current = std::make_shared<Node>(false, token_union);
        parent->AppendChild(current);

        // static || field
        if (!CompileKeyword(current, {KEYWORD_TYPE::STATIC, KEYWORD_TYPE::FIELD})) {
            THROW_COMPILER_EXCEPT
        }
        
        // type: int, char, boolean, class_name 
        MoveToNext();
        if (!CompileType(current)) {
            THROW_COMPILER_EXCEPT
        }

        // var name
        MoveToNext();
        if (!CompileIdentifier(current)) {
            THROW_COMPILER_EXCEPT
        }

        // ',' var name
        MoveToNext();
        while (CompileSymbol(nullptr, {","})) {
            if (!CompileSymbol(current, {","})) {
                THROW_COMPILER_EXCEPT
            }
            MoveToNext();

            if (!CompileIdentifier(current)) {
                THROW_COMPILER_EXCEPT
            }
            MoveToNext();
        }

        // ';'
        if (!CompileSymbol(current, {";"})) {
            THROW_COMPILER_EXCEPT
        }
    };

    void CompilationEngine::CompileSubroutine(std::shared_ptr<Node> parent) {
        Node::TokenUnion token_union;
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::SUBROUTINE_DEC;
        auto current = std::make_shared<Node>(false, token_union);
        parent->AppendChild(current);

        // constructor || function || method
        if (!CompileKeyword(current, {KEYWORD_TYPE::CONSTRUCTOR, KEYWORD_TYPE::FUNCTION, KEYWORD_TYPE::METHOD})) {
            THROW_COMPILER_EXCEPT
        }

        // void || type
        MoveToNext();
        if (!CompileKeyword(current, {KEYWORD_TYPE::VOID}) && !CompileType(current)) {
            THROW_COMPILER_EXCEPT
        }

        // subroutine name
        MoveToNext();
        if (!CompileIdentifier(current)) {
            THROW_COMPILER_EXCEPT
        }

        // ( 
        MoveToNext();
        if (!CompileSymbol(current, {"("})) {
            THROW_COMPILER_EXCEPT
        }

        // parameter list
        MoveToNext();
        CompileParameterList(current);

        // ) 
        if (!CompileSymbol(current, {")"})) {
            THROW_COMPILER_EXCEPT
        }

        // subroutine body
        MoveToNext();
        CompileSubroutineBody(current);
    };

    void CompilationEngine::CompileSubroutineBody(std::shared_ptr<Node> parent) {
        Node::TokenUnion token_union;
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::SUBROUTINE_BODY;
        auto current = std::make_shared<Node>(false, token_union);
        parent->AppendChild(current);

        // { 
        if (!CompileSymbol(current, {"{"})) {
            THROW_COMPILER_EXCEPT
        }

        // varDec
        MoveToNext();
        while (CompileKeyword(nullptr, {KEYWORD_TYPE::VAR})) {
            CompileVarDec(current);
            MoveToNext();
        }

        // statements
        CompileStatements(current);

        // } 
        if (!CompileSymbol(current, {"}"})) {
            THROW_COMPILER_EXCEPT
        }
    }

    void CompilationEngine::CompileParameterList(std::shared_ptr<Node> parent) {
        Node::TokenUnion token_union;
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::PARAMETER_LIST;
        auto current = std::make_shared<Node>(false, token_union);
        parent->AppendChild(current);

        if (!CompileType(current)) {
            return;
        }

        // type varName
        if (!CompileType(current)) {
            THROW_COMPILER_EXCEPT
        }

        MoveToNext();
        if (!CompileIdentifier(current)) {
            THROW_COMPILER_EXCEPT
        }

        // ',' type varName
        MoveToNext();
        while (CompileSymbol(nullptr, {","})) {
            if (!CompileSymbol(current, {","})) {
                THROW_COMPILER_EXCEPT
            }
            MoveToNext();
            if (!CompileType(current)) {
                THROW_COMPILER_EXCEPT
            }

            MoveToNext();
            if (!CompileIdentifier(current)) {
                THROW_COMPILER_EXCEPT
            }
            MoveToNext();
        }
    };

    void CompilationEngine::CompileVarDec(std::shared_ptr<Node> parent) {
        Node::TokenUnion token_union;
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::VARDEC;
        auto current = std::make_shared<Node>(false, token_union);
        parent->AppendChild(current);

        // var
        if (!CompileKeyword(current, {KEYWORD_TYPE::VAR})) {
            THROW_COMPILER_EXCEPT
        }

        // type
        MoveToNext();
        if (!CompileType(current)) {
            THROW_COMPILER_EXCEPT
        }

        // varName 
        MoveToNext();
        if (!CompileIdentifier(current)) {
            THROW_COMPILER_EXCEPT
        }
        
        // ',' varName
        MoveToNext();
        while (CompileSymbol(nullptr, {","})) {
            if (!CompileSymbol(current, {","})) {
                THROW_COMPILER_EXCEPT
            }
            MoveToNext();

            if (!CompileIdentifier(current)) {
                THROW_COMPILER_EXCEPT
            }
            MoveToNext();
        }

        // ';'
        if (!CompileSymbol(current, {";"})) {
            THROW_COMPILER_EXCEPT
        }
    };

    void CompilationEngine::CompileStatements(std::shared_ptr<Node> parent) {
        Node::TokenUnion token_union;
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::STATEMENTS;
        auto current = std::make_shared<Node>(false, token_union);
        parent->AppendChild(current);

        while (CompileKeyword(nullptr, {KEYWORD_TYPE::LET, KEYWORD_TYPE::IF, 
                KEYWORD_TYPE::WHILE, KEYWORD_TYPE::DO, KEYWORD_TYPE::RETURN})) {
            switch (tokenizer_.GetKeyword()) {
                case KEYWORD_TYPE::LET: {
                    CompileLet(current);
                    break;
                }
                case KEYWORD_TYPE::IF: {
                    CompileIf(current);
                    break;
                }
                case KEYWORD_TYPE::WHILE: {
                    CompileWhile(current);
                    break;
                }
                case KEYWORD_TYPE::DO: {
                    CompileDo(current);
                    break;
                }
                case KEYWORD_TYPE::RETURN: {
                    CompileReturn(current);
                    break;
                }
                default: {
                    THROW_COMPILER_EXCEPT
                }
            }
        }

    };

    void CompilationEngine::CompileDo(std::shared_ptr<Node> parent) {
        // todo unfinished
        Node::TokenUnion token_union;
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::DO_STATEMENT;
        auto current = std::make_shared<Node>(false, token_union);
        parent->AppendChild(current);

        // do
        if (!CompileKeyword(current, {KEYWORD_TYPE::DO})) {
            THROW_COMPILER_EXCEPT
        }

        // subroutineName
        MoveToNext();
        CompileSubroutineCall(current);

        MoveToNext();
        if (!CompileSymbol(current, {";"})) {
            THROW_COMPILER_EXCEPT
        }
        MoveToNext();
    };

    void CompilationEngine::CompileLet(std::shared_ptr<Node> parent) {
        Node::TokenUnion token_union;
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::LET_STATEMENT;
        auto current = std::make_shared<Node>(false, token_union);
        parent->AppendChild(current);

        // let
        if (!CompileKeyword(current, {KEYWORD_TYPE::LET})) {
            THROW_COMPILER_EXCEPT
        }

        // var name
        MoveToNext();
        if (!CompileIdentifier(current)) {
            THROW_COMPILER_EXCEPT
        }

        MoveToNext();
        if (CompileSymbol(nullptr, {"["})) {
            // [
            CompileSymbol(current, {"["});

            // expression
            MoveToNext();
            CompileExpression(current);

            // ]
            if (!CompileSymbol(current, {"]"})) {
                THROW_COMPILER_EXCEPT
            }
            MoveToNext();
        }

        // =
        if (!CompileSymbol(current, {"="})) {
            THROW_COMPILER_EXCEPT
        }

        // expression
        MoveToNext();
        CompileExpression(current);

        // ;
        if (!CompileSymbol(current, {";"})) {
            THROW_COMPILER_EXCEPT
        }
        MoveToNext();
    };

    void CompilationEngine::CompileWhile(std::shared_ptr<Node> parent) {
        Node::TokenUnion token_union;
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::WHILE_STATEMENT;
        auto current = std::make_shared<Node>(false, token_union);
        parent->AppendChild(current);

        // while
        if (!CompileKeyword(current, {KEYWORD_TYPE::WHILE})) {
            THROW_COMPILER_EXCEPT
        }

        // (
        MoveToNext();
        if (!CompileSymbol(current, {"("})) {
            THROW_COMPILER_EXCEPT
        }

        // expression
        MoveToNext();
        CompileExpression(current);

        // )
        if (!CompileSymbol(current, {")"})) {
            THROW_COMPILER_EXCEPT
        }

        // {
        MoveToNext();
        if (!CompileSymbol(current, {"{"})) {
            THROW_COMPILER_EXCEPT
        }

        // statements
        MoveToNext();
        CompileStatements(current);

        // }
        if (!CompileSymbol(current, {"}"})) {
            THROW_COMPILER_EXCEPT
        }
        MoveToNext();
    };

    void CompilationEngine::CompileReturn(std::shared_ptr<Node> parent) {
        Node::TokenUnion token_union;
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::RETURN_STATEMENT;
        auto current = std::make_shared<Node>(false, token_union);
        parent->AppendChild(current);

        // return
        if (!CompileKeyword(current, {KEYWORD_TYPE::RETURN})) {
            THROW_COMPILER_EXCEPT
        }

        MoveToNext();

        if (CompileSymbol(nullptr, {";"})) {
            // ;
        } else {
            // expression
            CompileExpression(current);
        }

        if (!CompileSymbol(current, {";"})) {
            THROW_COMPILER_EXCEPT
        }
        MoveToNext();
    };

    void CompilationEngine::CompileIf(std::shared_ptr<Node> parent) {
        Node::TokenUnion token_union;
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::IF_STATEMENT;
        auto current = std::make_shared<Node>(false, token_union);
        parent->AppendChild(current);

        // if
        if (!CompileKeyword(current, {KEYWORD_TYPE::IF})) {
            THROW_COMPILER_EXCEPT
        }

        // (
        MoveToNext();
        if (!CompileSymbol(current, {"("})) {
            THROW_COMPILER_EXCEPT
        }

        // expression
        MoveToNext();
        CompileExpression(current);

        // )
        if (!CompileSymbol(current, {")"})) {
            THROW_COMPILER_EXCEPT
        }

        // {
        MoveToNext();
        if (!CompileSymbol(current, {"{"})) {
            THROW_COMPILER_EXCEPT
        }

        // statements
        MoveToNext();
        CompileStatements(current);

        // }
        if (!CompileSymbol(current, {"}"})) {
            THROW_COMPILER_EXCEPT
        }

        MoveToNext();
        if (CompileKeyword(nullptr, {KEYWORD_TYPE::ELSE})) {

            if (!CompileKeyword(current, {KEYWORD_TYPE::ELSE})) {
                THROW_COMPILER_EXCEPT
            }

            // {
            MoveToNext();
            if (!CompileSymbol(current, {"{"})) {
                THROW_COMPILER_EXCEPT
            }

            // statements
            MoveToNext();
            CompileStatements(current);

            // }
            if (!CompileSymbol(current, {"}"})) {
                THROW_COMPILER_EXCEPT
            }
            MoveToNext();
        }
    };

    void CompilationEngine::CompileExpression(std::shared_ptr<Node> parent) {
        Node::TokenUnion token_union;
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::EXPRESSION;
        auto current = std::make_shared<Node>(false, token_union);
        parent->AppendChild(current);

        //term 
        CompileTerm(current);

        // op term
        while (CompileOp(nullptr)) {
            if (!CompileOp(current)) {
                THROW_COMPILER_EXCEPT
            }

            MoveToNext();
            CompileTerm(current);
        }
    };

    void CompilationEngine::CompileTerm(std::shared_ptr<Node> parent) {
        Node::TokenUnion token_union;
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::TERM;
        auto current = std::make_shared<Node>(false, token_union);
        parent->AppendChild(current);

        if (CompileIntegerConst(current) ||
            CompileStringConst(current) ||
            CompileKeyword(current, {KEYWORD_TYPE::TRUE, KEYWORD_TYPE::FALSE, KEYWORD_TYPE::NULL_, KEYWORD_TYPE::THIS})) {
            MoveToNext();
            return;
        }

        if (CompileIdentifier(nullptr)) {
            CompileIdentifier(current);
            MoveToNext();

            if (CompileSymbol(nullptr, {"["})) {
                // var name ('[' expression ']')
                CompileSymbol(current, {"["});

                MoveToNext();
                CompileExpression(current);

                if (!CompileSymbol(current, {"]"})) {
                    THROW_COMPILER_EXCEPT
                }
                MoveToNext();
            } else if (CompileSymbol(nullptr, {"(", "."})) {
                // subroutine call
                if (CompileSymbol(nullptr, {"."})) {
                    CompileSymbol(current, {"."});

                    MoveToNext();
                    if (!CompileIdentifier(current)) {
                        THROW_COMPILER_EXCEPT
                    }

                    MoveToNext();
                }

                // (
                if (!CompileSymbol(current, {"("})) {
                    THROW_COMPILER_EXCEPT
                }

                // expression list
                MoveToNext();
                CompileExpressionList(current);

                // )
                if (!CompileSymbol(current, {")"})) {
                    THROW_COMPILER_EXCEPT
                }

                MoveToNext();
            }
        } else if (CompileSymbol(nullptr, {"("})) {
            // ( expression )
            if (!CompileSymbol(current, {"("})) {
                THROW_COMPILER_EXCEPT
            }

            MoveToNext();
            CompileExpression(current);

            if (!CompileSymbol(current, {")"})) {
                THROW_COMPILER_EXCEPT
            }
            MoveToNext();

        } else if (CompileUnaryOp(nullptr)) {
            // unary op term
            if (!CompileUnaryOp(current)) {
                THROW_COMPILER_EXCEPT
            }

            MoveToNext();
            CompileTerm(current);
        } else {
            THROW_COMPILER_EXCEPT
        }
    };

    void CompilationEngine::CompileExpressionList(std::shared_ptr<Node> parent) {
        Node::TokenUnion token_union;
        token_union.non_terminal_token_ = NON_TERMINAL_TOKEN_TYPE::EXPRESSION_LIST;
        auto current = std::make_shared<Node>(false, token_union);
        parent->AppendChild(current);

        try {
            CompileExpression(current);
        } catch (CompileException) {
            return;
        }

        while (CompileSymbol(nullptr, {","})) {
            if (CompileSymbol(current, {","})) {
                THROW_COMPILER_EXCEPT
            }

            MoveToNext();
            CompileExpression(current);
        }
    };

    bool CompilationEngine::CompileOp(std::shared_ptr<Node> parent) {
        return CompileSymbol(parent, {"+", "-", "*", "/", "&", "|", "<", ">", "="});
    }

    bool CompilationEngine::CompileUnaryOp(std::shared_ptr<Node> parent) {
        return CompileSymbol(parent, {"-", "~"});
    }

    bool CompilationEngine::CompileType(std::shared_ptr<Node> parent) {
        if (!CompileKeyword(parent, {KEYWORD_TYPE::INT, KEYWORD_TYPE::CHAR, KEYWORD_TYPE::BOOLEAN}) &&
            !CompileIdentifier(parent)) {
            return false;
        }
        return true;
    }

    void CompilationEngine::CompileSubroutineCall(std::shared_ptr<Node> parent) {
        if (!CompileIdentifier(parent)) {
            THROW_COMPILER_EXCEPT
        }

        MoveToNext();
        if (CompileSymbol(nullptr, {"."})) {
            if (!CompileSymbol(parent, {"."})) {
                THROW_COMPILER_EXCEPT
            }

            MoveToNext();
            if (!CompileIdentifier(parent)) {
                THROW_COMPILER_EXCEPT
            }

            MoveToNext();
        }

        // (
        if (!CompileSymbol(parent, {"("})) {
            THROW_COMPILER_EXCEPT
        }

        // expression list
        MoveToNext();
        CompileExpressionList(parent);

        // )
        if (!CompileSymbol(parent, {")"})) {
            THROW_COMPILER_EXCEPT
        }
    }

    bool CompilationEngine::CompileKeyword(std::shared_ptr<Node> parent, std::vector<KEYWORD_TYPE> limit_types) {
        if (tokenizer_.GetTokenType() != TERMINAL_TOKEN_TYPE::KEYWORD || 
            std::find(limit_types.begin(), limit_types.end(), tokenizer_.GetKeyword()) == limit_types.end()) {
            return false;
        }
        if (parent == nullptr) {
            return true;
        }
        Node::TokenUnion token_union = {TERMINAL_TOKEN_TYPE::KEYWORD};
        auto node = std::make_shared<Node>(true, token_union, KEYWORD_STR_TABLE.at(tokenizer_.GetKeyword()));
        parent->AppendChild(node);
        return true;
    }

    bool CompilationEngine::CompileSymbol(std::shared_ptr<Node> parent, std::vector<std::string> limit_symbols) {
        if (tokenizer_.GetTokenType() != TERMINAL_TOKEN_TYPE::SYMBOL || 
            std::find(limit_symbols.begin(), limit_symbols.end(), tokenizer_.GetSymbol()) == limit_symbols.end()) {
            return false;
        }
        if (parent == nullptr) {
            return true;
        }
        Node::TokenUnion token_union = {TERMINAL_TOKEN_TYPE::SYMBOL};

        std::string valid_symbol = tokenizer_.GetSymbol();
        if (valid_symbol == ">") {
            valid_symbol = "&gt;";
        } else if (valid_symbol == "<") {
            valid_symbol = "&lt;";
        } else if (valid_symbol == "&") {
            valid_symbol = "&amp;";
        }
        auto node = std::make_shared<Node>(true, token_union, valid_symbol);
        parent->AppendChild(node);
        return true;
    }
    
    bool CompilationEngine::CompileIdentifier(std::shared_ptr<Node> parent) {
        if (tokenizer_.GetTokenType() != TERMINAL_TOKEN_TYPE::IDENTIFIER) {
            return false;
        }
        if (parent == nullptr) {
            return true;
        }
        Node::TokenUnion token_union = {TERMINAL_TOKEN_TYPE::IDENTIFIER};
        auto node = std::make_shared<Node>(true, token_union, tokenizer_.GetIdentifier());
        parent->AppendChild(node);
        return true;
    }

    bool CompilationEngine::CompileIntegerConst(std::shared_ptr<Node> parent) {
        if (tokenizer_.GetTokenType() != TERMINAL_TOKEN_TYPE::INT_CONST) {
            return false;
        }
        if (parent == nullptr) {
            return true;
        }
        Node::TokenUnion token_union = {TERMINAL_TOKEN_TYPE::INT_CONST};
        auto node = std::make_shared<Node>(true, token_union, std::to_string(tokenizer_.GetIntVal()));
        parent->AppendChild(node);
        return true;
    }

    bool CompilationEngine::CompileStringConst(std::shared_ptr<Node> parent) {
        if (tokenizer_.GetTokenType() != TERMINAL_TOKEN_TYPE::STRING_CONST) {
            return false;
        }
        if (parent == nullptr) {
            return true;
        }
        Node::TokenUnion token_union = {TERMINAL_TOKEN_TYPE::STRING_CONST};
        auto node = std::make_shared<Node>(true, token_union, tokenizer_.GetStringVal());
        parent->AppendChild(node);
        return true;
    }
} // namespace jack_compiler
