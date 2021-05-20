#include <algorithm>
#include <memory>

#include "compilation_engine/compilation_engine.h"
#include "exception/exception.h"
#include "utils/enum/enum.h"
#include "writer/xml/xml_writer.h"
#include "writer/vm/vm_writer.h"

namespace jack_compiler {
template<class WriterType>
CompilationEngine<WriterType>::CompilationEngine(std::istream* input, std::ostream* output): tokenizer_(input) {
    writer_ = new WriterType(output);
}

template<class WriterType>
CompilationEngine<WriterType>::CompilationEngine(const std::string& input_file, const std::string& output_file):
                                    tokenizer_(input_file) {
    writer_ = new WriterType(output_file);
}

template<class WriterType>
CompilationEngine<WriterType>::~CompilationEngine() {
    delete writer_;
}

template<class WriterType>
void CompilationEngine<WriterType>::MoveToNext() {
    if (!tokenizer_.HasMoreTokens()) {
        THROW_COMPILER_EXCEPT
    }
    tokenizer_.Advance();
}

template<class WriterType>
void CompilationEngine<WriterType>::CompileClass() {
    Node::TokenUnion token_union;
    token_union.non_terminal_token_ = NonTerminalTokenType::kClass;
    root_ = std::make_shared<Node>(false, token_union);

    // class
    MoveToNext();
    if (!CompileKeyword(root_, {KeywordType::kClass})) {
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
    while (CompileKeyword(nullptr, {KeywordType::kField, KeywordType::kStatic})) {
        CompileClassVarDec(root_);
        MoveToNext();
    }

    // class subroutine
    while (CompileKeyword(nullptr, {KeywordType::kConstructor, KeywordType::kFunction, KeywordType::kMethod})) {
        CompileSubroutine(root_);
        MoveToNext();
    }

    // }
    if (!CompileSymbol(root_, {"}"})) {
        THROW_COMPILER_EXCEPT
    }
    writer_->WriteToOutput(root_);
}

template<class WriterType>
void CompilationEngine<WriterType>::CompileClassVarDec(std::shared_ptr<Node> parent) {
    Node::TokenUnion token_union;
    token_union.non_terminal_token_ = NonTerminalTokenType::kClassVarDec;
    auto current = std::make_shared<Node>(false, token_union);
    parent->AppendChild(current);
    // static || field
    if (!CompileKeyword(current, {KeywordType::kStatic, KeywordType::kField})) {
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
}

template<class WriterType>
void CompilationEngine<WriterType>::CompileSubroutine(std::shared_ptr<Node> parent) {
    Node::TokenUnion token_union;
    token_union.non_terminal_token_ = NonTerminalTokenType::kSubroutineDec;
    auto current = std::make_shared<Node>(false, token_union);
    parent->AppendChild(current);

    // constructor || function || method
    if (!CompileKeyword(current, {KeywordType::kConstructor, KeywordType::kFunction, KeywordType::kMethod})) {
        THROW_COMPILER_EXCEPT
    }

    // void || type
    MoveToNext();
    if (!CompileKeyword(current, {KeywordType::kVoid}) && !CompileType(current)) {
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
}

template<class WriterType>
void CompilationEngine<WriterType>::CompileSubroutineBody(std::shared_ptr<Node> parent) {
    Node::TokenUnion token_union;
    token_union.non_terminal_token_ = NonTerminalTokenType::kSubroutineBody;
    auto current = std::make_shared<Node>(false, token_union);
    parent->AppendChild(current);

    // {
    if (!CompileSymbol(current, {"{"})) {
        THROW_COMPILER_EXCEPT
    }

    // varDec
    MoveToNext();
    while (CompileKeyword(nullptr, {KeywordType::kVar})) {
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

template<class WriterType>
void CompilationEngine<WriterType>::CompileParameterList(std::shared_ptr<Node> parent) {
    Node::TokenUnion token_union;
    token_union.non_terminal_token_ = NonTerminalTokenType::kParameterList;
    auto current = std::make_shared<Node>(false, token_union);
    parent->AppendChild(current);

    if (!CompileType(nullptr)) {
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
}

template<class WriterType>
void CompilationEngine<WriterType>::CompileVarDec(std::shared_ptr<Node> parent) {
    Node::TokenUnion token_union;
    token_union.non_terminal_token_ = NonTerminalTokenType::kVarDec;
    auto current = std::make_shared<Node>(false, token_union);
    parent->AppendChild(current);

    // var
    if (!CompileKeyword(current, {KeywordType::kVar})) {
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
}

template<class WriterType>
void CompilationEngine<WriterType>::CompileStatements(std::shared_ptr<Node> parent) {
    Node::TokenUnion token_union;
    token_union.non_terminal_token_ = NonTerminalTokenType::kStatements;
    auto current = std::make_shared<Node>(false, token_union);
    parent->AppendChild(current);

    while (CompileKeyword(nullptr, {KeywordType::kLet, KeywordType::kIf,
            KeywordType::kWhile, KeywordType::kDo, KeywordType::kReturn})) {
        switch (tokenizer_.GetKeyword()) {
            case KeywordType::kLet: {
                CompileLet(current);
                break;
            }
            case KeywordType::kIf: {
                CompileIf(current);
                break;
            }
            case KeywordType::kWhile: {
                CompileWhile(current);
                break;
            }
            case KeywordType::kDo: {
                CompileDo(current);
                break;
            }
            case KeywordType::kReturn: {
                CompileReturn(current);
                break;
            }
            default: {
                THROW_COMPILER_EXCEPT
            }
        }
    }
}

template<class WriterType>
void CompilationEngine<WriterType>::CompileDo(std::shared_ptr<Node> parent) {
    // todo unfinished
    Node::TokenUnion token_union;
    token_union.non_terminal_token_ = NonTerminalTokenType::kDoStatement;
    auto current = std::make_shared<Node>(false, token_union);
    parent->AppendChild(current);

    // do
    if (!CompileKeyword(current, {KeywordType::kDo})) {
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
}

template<class WriterType>
void CompilationEngine<WriterType>::CompileLet(std::shared_ptr<Node> parent) {
    Node::TokenUnion token_union;
    token_union.non_terminal_token_ = NonTerminalTokenType::kLetStatement;
    auto current = std::make_shared<Node>(false, token_union);
    parent->AppendChild(current);

    // let
    if (!CompileKeyword(current, {KeywordType::kLet})) {
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
}

template<class WriterType>
void CompilationEngine<WriterType>::CompileWhile(std::shared_ptr<Node> parent) {
    Node::TokenUnion token_union;
    token_union.non_terminal_token_ = NonTerminalTokenType::kWhileStatement;
    auto current = std::make_shared<Node>(false, token_union);
    parent->AppendChild(current);

    // while
    if (!CompileKeyword(current, {KeywordType::kWhile})) {
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
}

template<class WriterType>
void CompilationEngine<WriterType>::CompileReturn(std::shared_ptr<Node> parent) {
    Node::TokenUnion token_union;
    token_union.non_terminal_token_ = NonTerminalTokenType::kReturnStatement;
    auto current = std::make_shared<Node>(false, token_union);
    parent->AppendChild(current);

    // return
    if (!CompileKeyword(current, {KeywordType::kReturn})) {
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
}

template<class WriterType>
void CompilationEngine<WriterType>::CompileIf(std::shared_ptr<Node> parent) {
    Node::TokenUnion token_union;
    token_union.non_terminal_token_ = NonTerminalTokenType::kIfStatement;
    auto current = std::make_shared<Node>(false, token_union);
    parent->AppendChild(current);

    // if
    if (!CompileKeyword(current, {KeywordType::kIf})) {
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
    if (CompileKeyword(nullptr, {KeywordType::kElse})) {
        if (!CompileKeyword(current, {KeywordType::kElse})) {
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
}

template<class WriterType>
void CompilationEngine<WriterType>::CompileExpression(std::shared_ptr<Node> parent) {
    Node::TokenUnion token_union;
    token_union.non_terminal_token_ = NonTerminalTokenType::kExpression;
    auto current = std::make_shared<Node>(false, token_union);
    parent->AppendChild(current);

    // term
    CompileTerm(current);

    // op term
    while (CompileOp(nullptr)) {
        if (!CompileOp(current)) {
            THROW_COMPILER_EXCEPT
        }

        MoveToNext();
        CompileTerm(current);
    }
}

template<class WriterType>
void CompilationEngine<WriterType>::CompileTerm(std::shared_ptr<Node> parent) {
    Node::TokenUnion token_union;
    token_union.non_terminal_token_ = NonTerminalTokenType::kTerm;
    auto current = std::make_shared<Node>(false, token_union);
    parent->AppendChild(current);

    if (CompileIntegerConst(current) ||
        CompileStringConst(current) ||
        CompileKeyword(current, {KeywordType::kTrue, KeywordType::kFalse, KeywordType::kNull, KeywordType::kThis})) {
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
}

template<class WriterType>
void CompilationEngine<WriterType>::CompileExpressionList(std::shared_ptr<Node> parent) {
    Node::TokenUnion token_union;
    token_union.non_terminal_token_ = NonTerminalTokenType::kExpressionList;
    auto current = std::make_shared<Node>(false, token_union);
    auto tmp = std::make_shared<Node>(false, token_union);

    try {
        CompileExpression(current);
        parent->AppendChild(current);
    } catch (...) {
        parent->AppendChild(tmp);
        return;
    }

    while (CompileSymbol(nullptr, {","})) {
        if (!CompileSymbol(current, {","})) {
            THROW_COMPILER_EXCEPT
        }

        MoveToNext();
        CompileExpression(current);
    }
}

template<class WriterType>
bool CompilationEngine<WriterType>::CompileOp(std::shared_ptr<Node> parent) {
    return CompileSymbol(parent, {"+", "-", "*", "/", "&", "|", "<", ">", "="});
}

template<class WriterType>
bool CompilationEngine<WriterType>::CompileUnaryOp(std::shared_ptr<Node> parent) {
    return CompileSymbol(parent, {"-", "~"});
}

template<class WriterType>
bool CompilationEngine<WriterType>::CompileType(std::shared_ptr<Node> parent) {
    if (!CompileKeyword(parent, {KeywordType::kInt, KeywordType::kChar, KeywordType::kBoolean}) &&
        !CompileIdentifier(parent)) {
        return false;
    }
    return true;
}

template<class WriterType>
void CompilationEngine<WriterType>::CompileSubroutineCall(std::shared_ptr<Node> parent) {
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

template<class WriterType>
bool CompilationEngine<WriterType>::CompileKeyword(std::shared_ptr<Node> parent, std::vector<KeywordType> limit_types) {
    if (tokenizer_.GetTokenType() != TerminalTokenType::kKeyword ||
        std::find(limit_types.begin(), limit_types.end(), tokenizer_.GetKeyword()) == limit_types.end()) {
        return false;
    }
    if (parent == nullptr) {
        return true;
    }
    Node::TokenUnion token_union = {TerminalTokenType::kKeyword};
    auto node = std::make_shared<Node>(true, token_union, KeywordTypeString.at(tokenizer_.GetKeyword()));
    parent->AppendChild(node);
    return true;
}

template<class WriterType>
bool CompilationEngine<WriterType>::CompileSymbol(std::shared_ptr<Node> parent, std::vector<std::string> limit_symbols) {
    if (tokenizer_.GetTokenType() != TerminalTokenType::kSymbol ||
        std::find(limit_symbols.begin(), limit_symbols.end(), tokenizer_.GetSymbol()) == limit_symbols.end()) {
        return false;
    }
    if (parent == nullptr) {
        return true;
    }
    Node::TokenUnion token_union = {TerminalTokenType::kSymbol};
    auto node = std::make_shared<Node>(true, token_union, tokenizer_.GetSymbol());
    parent->AppendChild(node);
    return true;
}

template<class WriterType>
bool CompilationEngine<WriterType>::CompileIdentifier(std::shared_ptr<Node> parent) {
    if (tokenizer_.GetTokenType() != TerminalTokenType::kIdentifier) {
        return false;
    }
    if (parent == nullptr) {
        return true;
    }
    Node::TokenUnion token_union = {TerminalTokenType::kIdentifier};
    auto node = std::make_shared<Node>(true, token_union, tokenizer_.GetIdentifier());
    parent->AppendChild(node);
    return true;
}

template<class WriterType>
bool CompilationEngine<WriterType>::CompileIntegerConst(std::shared_ptr<Node> parent) {
    if (tokenizer_.GetTokenType() != TerminalTokenType::kIntConst) {
        return false;
    }
    if (parent == nullptr) {
        return true;
    }
    Node::TokenUnion token_union = {TerminalTokenType::kIntConst};
    auto node = std::make_shared<Node>(true, token_union, std::to_string(tokenizer_.GetIntVal()));
    parent->AppendChild(node);
    return true;
}

template<class WriterType>
bool CompilationEngine<WriterType>::CompileStringConst(std::shared_ptr<Node> parent) {
    if (tokenizer_.GetTokenType() != TerminalTokenType::kStringConst) {
        return false;
    }
    if (parent == nullptr) {
        return true;
    }
    Node::TokenUnion token_union = {TerminalTokenType::kStringConst};
    auto node = std::make_shared<Node>(true, token_union, tokenizer_.GetStringVal());
    parent->AppendChild(node);
    return true;
}

template class CompilationEngine<XMLWriter>;
template class CompilationEngine<VMWriter>;
}  // namespace jack_compiler
