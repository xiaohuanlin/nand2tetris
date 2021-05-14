#include "writer/vm/vm_writer.h"

namespace jack_compiler {
void VMWriter::WritePush(const SegmentType &seg, const int &index) {
    string_stream_ << "push " << SegmentTypeString.at(seg) << " " << index << std::endl;
}

void VMWriter::WritePop(const SegmentType &seg, const int &index) {
    string_stream_ << "pop" << SegmentTypeString.at(seg) << " " << index << std::endl;
}

void VMWriter::WriteArithmetic(const ArithmeticType &arithmetic_command) {
    string_stream_ << ArithmeticTypeString.at(arithmetic_command) << std::endl;
}

void VMWriter::WriteLabel(const std::string &label) {
    string_stream_ << "label " << label << std::endl;
}

void VMWriter::WriteGoto(const std::string &label) {
    string_stream_ << "goto " << label << std::endl;
}

void VMWriter::WriteIf(const std::string &label) {
    string_stream_ << "if-goto " << label << std::endl;
}

void VMWriter::WriteCall(const std::string &name, const int &n_args) {
    string_stream_ << "call " << name << " " << n_args << std::endl;
}

void VMWriter::WriteFunction(const std::string &name, const int &n_args) {
    string_stream_ << "function" << name << " " << n_args << std::endl;
}

void VMWriter::WriteReturn() {
    string_stream_ << "return" << std::endl;
}

std::string VMWriter::Write(const std::shared_ptr<Node> &root, __attribute__((unused)) int level) {
    WriteClass(root);
    return string_stream_.str();
}

void VMWriter::WriteClass(const std::shared_ptr<Node> &root) {
    auto iter = root->GetChildren().begin();
    ++iter;
    class_name_ = TERMINAL_TOKEN_TABLE.at((*iter)->GetTokenType().terminal_token_);
    ++iter;
    ++iter;
    if ((*iter)->GetTokenType().non_terminal_token_ == NON_TERMINAL_TOKEN_TYPE::CLASS_VAR_DEC) {
        WriteClassVarDec(*iter);
        ++iter;
    }

    if ((*iter)->GetTokenType().non_terminal_token_ == NON_TERMINAL_TOKEN_TYPE::SUBROUTINE_DEC) {
        WriteClassVarDec(*iter);
        ++iter;
    }
}

void VMWriter::WriteClassVarDec(const std::shared_ptr<Node> &root) {
    auto iter = root->GetChildren().begin();
    SymbolTableKind kind;
    if ((*iter)->GetContent() == "static") {
        kind = SymbolTableKind::kStatic;
    } else {
        kind = SymbolTableKind::kField;
    }

    ++iter;
    SymbolTable::Type type = (*iter)->GetContent();

    ++iter;
    SymbolTable::Name name = (*iter)->GetContent();
    symbol_table_.Define(name, type, kind);

    ++iter;
    while ((*iter)->GetContent() == ",") {
        ++iter;
        name = (*iter)->GetContent();
        symbol_table_.Define(name, type, kind);
    }
}

void VMWriter::WriteClassSubroutineDec(const std::shared_ptr<Node> &root) {
    symbol_table_.StartSubroutine();
    auto iter = root->GetChildren().begin();
    ++iter;
    ++iter;
    function_name_ = class_name_ + "." + (*iter)->GetContent();

    ++iter;
    ++iter;
    WriteParameterList(*iter);

    int parameter_count = ((*iter)->GetChildren().size() + 1) / 3;
    WriteFunction(function_name_, parameter_count);

    ++iter;
    ++iter;
    WriteSubroutineBody(*iter);
}

void VMWriter::WriteParameterList(const std::shared_ptr<Node> &root) {
    if (root->GetChildren().empty()) {
        return;
    }
    auto iter = root->GetChildren().begin();
    SymbolTable::Type type = (*iter)->GetContent();

    ++iter;
    SymbolTable::Name name = (*iter)->GetContent();
    symbol_table_.Define(name, type, SymbolTableKind::kArg);

    ++iter;
    while ((*iter)->GetContent() == ",") {
        ++iter;
        type = (*iter)->GetContent();

        ++iter;
        name = (*iter)->GetContent();
        symbol_table_.Define(name, type, SymbolTableKind::kArg);

        ++iter;
    }
}

void VMWriter::WriteSubroutineBody(const std::shared_ptr<Node> &root) {
    auto iter = root->GetChildren().begin();

    ++iter;
    while ((*iter)->GetTokenType().non_terminal_token_ == NON_TERMINAL_TOKEN_TYPE::VARDEC) {
        WriteSubroutineVarDec(*iter);
        ++iter;
    }
    
    WriteStatements(*iter);
}

void VMWriter::WriteSubroutineVarDec(const std::shared_ptr<Node> &root) {
    auto iter = root->GetChildren().begin();

    ++iter;
    SymbolTable::Type type = (*iter)->GetContent();

    ++iter;
    SymbolTable::Name name = (*iter)->GetContent();
    symbol_table_.Define(name, type, SymbolTableKind::kVar);

    ++iter;
    while ((*iter)->GetContent() == ",") {
        ++iter;
        name = (*iter)->GetContent();
        symbol_table_.Define(name, type, SymbolTableKind::kVar);

        ++iter;
    }
}

void VMWriter::WriteStatements(const std::shared_ptr<Node> &root) {
    if (root->GetChildren().empty()) {
        return;
    }

    for (auto node: root->GetChildren()) {
        switch (node->GetTokenType().non_terminal_token_) {
            case NON_TERMINAL_TOKEN_TYPE::LET_STATEMENT: {
                WriteLetStatement(node);
                break;
            }
            case NON_TERMINAL_TOKEN_TYPE::DO_STATEMENT: {
                WriteDoStatement(node);
                break;
            }
            case NON_TERMINAL_TOKEN_TYPE::WHILE_STATEMENT: {
                WriteWhileStatement(node);
                break;
            }
            case NON_TERMINAL_TOKEN_TYPE::IF_STATEMENT: {
                WriteIfStatement(node);
                break;
            }
            case NON_TERMINAL_TOKEN_TYPE::RETURN_STATEMENT: {
                WriteReturnStatement(node);
                break;
            }
            default: {
                break;
            }
        }
    }
}

void VMWriter::WriteLetStatement(const std::shared_ptr<Node> &root) {
    auto iter = root->GetChildren().begin();
    ++iter;

    std::string var_name = (*iter)->GetContent();
    auto kind = symbol_table_.KindOf(var_name);
    auto index = symbol_table_.IndexOf(var_name);
    ++iter;
    if ((*iter)->GetContent() == "[") {
        WritePush(SymbolTableKindToSegmentType.at(kind), index);
        ++iter;
        WriteExpression(*iter);
        // calculate the postion of array[expression]
        WriteArithmetic(ArithmeticType::kAdd);
        // put it in "that" pointer
        WritePop(SegmentType::kPointer, 1);
        ++iter;
        ++iter;
        ++iter;
        WriteExpression(*iter);
        WritePop(SegmentType::kThat, 0);
    } else {
        ++iter;
        WriteExpression(*iter);
        WritePop(SymbolTableKindToSegmentType.at(kind), index);
    }
}

void VMWriter::WriteDoStatement(const std::shared_ptr<Node> &root) {
    auto iter = root->GetChildren().begin();
    ++iter;

    std::string class_or_var_name = class_name_;
    std::string func_name = (*iter)->GetContent();
    int parameter_count = 0;

    ++iter;
    if ((*iter)->GetContent() == ".") {
        ++iter;
        class_or_var_name = func_name;
        func_name = (*iter)->GetContent();

        try {
            // we should push "this" pointer into stack if it is a variable
            auto kind = symbol_table_.KindOf(class_or_var_name);
            auto index = symbol_table_.IndexOf(class_or_var_name);
            WritePush(SymbolTableKindToSegmentType.at(kind), index);
            ++parameter_count;
        } catch (...) {
        }

        ++iter;
    }
    ++iter;
    WriteExpressionList(*iter);
    parameter_count += ((*iter)->GetChildren().size() + 1) / 2;
    WriteCall(class_name_ + "." + func_name, parameter_count);
}

void VMWriter::WriteWhileStatement(const std::shared_ptr<Node> &root) {
    auto iter = root->GetChildren().begin();
    ++label_count_;
    WriteLabel(function_name_ + std::to_string(label_count_) + "START");
    ++iter;
    ++iter;
    WriteExpression(*iter);
    WriteArithmetic(ArithmeticType::kNeg);
    ++iter;
    ++iter;
    WriteIf(function_name_ + std::to_string(label_count_) + "NEXT");
    WriteStatements(*iter);
    WriteGoto(function_name_ + std::to_string(label_count_) + "START");
    WriteLabel(function_name_ + std::to_string(label_count_) + "NEXT");
}

void VMWriter::WriteIfStatement(const std::shared_ptr<Node> &root) {
    auto iter = root->GetChildren().begin();
    ++label_count_;

    ++iter;
    ++iter;
    WriteExpression(*iter);
    WriteArithmetic(ArithmeticType::kNeg);
    ++iter;
    ++iter;
    WriteIf(function_name_ + std::to_string(label_count_) + "NEXT");
    // true statements
    WriteStatements(*iter);
    ++iter;
    ++iter;
    WriteGoto(function_name_ + std::to_string(label_count_) + "REMAIN");
    WriteLabel(function_name_ + std::to_string(label_count_) + "NEXT");
    if (iter != root->GetChildren().end()) {
        // else part
        ++iter;
        ++iter;
        // false statements
        WriteStatements(*iter);
    }
    WriteLabel(function_name_ + std::to_string(label_count_) + "REMAIN");
}

void VMWriter::WriteReturnStatement(const std::shared_ptr<Node> &root) {
    auto iter = root->GetChildren().begin();
    ++iter;

    if ((*iter)->GetTokenType().non_terminal_token_ == NON_TERMINAL_TOKEN_TYPE::EXPRESSION) {
        WriteExpression(*iter);
    } else {
        // push 0 for void return
        WritePush(SegmentType::kConst, 0);
    }

    WriteReturn();
}

void VMWriter::WriteExpressionList(const std::shared_ptr<Node> &root) {
    if (root->GetChildren().empty()) {
        return;
    }

    auto iter = root->GetChildren().begin();
    WriteExpression(*iter);
    while (iter != root->GetChildren().end()) {
        ++iter;
        WriteExpression(*iter);
    }

}

void VMWriter::WriteExpression(const std::shared_ptr<Node> &root) {
    auto iter = root->GetChildren().begin();
    WriteTerm(*iter);
    ++iter;
    while (iter != root->GetChildren().end()) {
        OpType op = StringOpType.at((*iter)->GetContent());
        WriteTerm(*iter);
        switch (op) {
            case OpType::kAdd: {
                WriteArithmetic(ArithmeticType::kAdd);
                break;
            }
            case OpType::kSub: {
                WriteArithmetic(ArithmeticType::kSub);
                break;
            }
            case OpType::kMultiply: {
                WriteCall("Math.multiply", 2);
                break;
            }
            case OpType::kDivide: {
                WriteCall("Math.divide", 2);
                break;
            }
            case OpType::kEq: {
                WriteArithmetic(ArithmeticType::kEq);
                break;
            }
            case OpType::kGt: {
                WriteArithmetic(ArithmeticType::kGt);
                break;
            }
            case OpType::kLt: {
                WriteArithmetic(ArithmeticType::kLt);
                break;
            }
            case OpType::kAnd: {
                WriteArithmetic(ArithmeticType::kAnd);
                break;
            }
            case OpType::kOr: {
                WriteArithmetic(ArithmeticType::kOr);
                break;
            }
            default: {
                break;
            }
        }
        ++iter;
    }
}

void VMWriter::WriteTerm(const std::shared_ptr<Node> &root) {
    auto iter = root->GetChildren().begin();
    auto terminal_token = (*iter)->GetTokenType().terminal_token_;
    switch (terminal_token) {
        case TERMINAL_TOKEN_TYPE::INT_CONST: {
            WritePush(SegmentType::kConst, std::stod((*iter)->GetContent()));
            break;
        }
        case TERMINAL_TOKEN_TYPE::STRING_CONST: {
            auto const_string = (*iter)->GetContent();
            std::size_t length = const_string.length();
            WritePush(SegmentType::kConst, length);
            WriteCall("String.new", 1);
            for (std::size_t i = 0; i < length; ++i) {
                WritePush(SegmentType::kConst, static_cast<int>(const_string[i]));
                WriteCall("String.appendChar", 2);
            }
            break;
        }
        case TERMINAL_TOKEN_TYPE::KEYWORD: {
            // for "true" and "false" keyword
            if ((*iter)->GetContent() == "true") {
                WritePush(SegmentType::kConst, 1);
                WriteArithmetic(ArithmeticType::kNeg);
            } else {
                WritePush(SegmentType::kConst, 0);
            }
            break;
        }
        case TERMINAL_TOKEN_TYPE::IDENTIFIER: {
            // varname | varname[expression] | subroutineCall
            auto varname = (*iter)->GetContent();
            ++iter;
            if ((*iter)->GetChildren().empty()) {
                // varname
                auto kind = symbol_table_.KindOf(varname);
                auto index = symbol_table_.IndexOf(varname);
                WritePush(SymbolTableKindToSegmentType.at(kind), index);
            } else if ((*iter)->GetContent() == "[") {
                // varname[expression]
                auto kind = symbol_table_.KindOf(varname);
                auto index = symbol_table_.IndexOf(varname);
                WritePush(SymbolTableKindToSegmentType.at(kind), index);
                ++iter;
                WriteExpression(*iter);
                // calculate the postion of array[expression]
                WriteArithmetic(ArithmeticType::kAdd);
                // put it in "that" pointer
                WritePop(SegmentType::kPointer, 1);
                WritePush(SegmentType::kThat, 0);
            } else {
                // subroutineCall
                std::string class_or_var_name = class_name_;
                std::string func_name = (*iter)->GetContent();
                int parameter_count = 0;

                ++iter;
                if ((*iter)->GetContent() == ".") {
                    ++iter;
                    class_or_var_name = func_name;
                    func_name = (*iter)->GetContent();

                    try {
                        // we should push "this" pointer into stack if it is a variable
                        auto kind = symbol_table_.KindOf(class_or_var_name);
                        auto index = symbol_table_.IndexOf(class_or_var_name);
                        WritePush(SymbolTableKindToSegmentType.at(kind), index);
                        ++parameter_count;
                    } catch (...) {
                    }

                    ++iter;
                }
                ++iter;
                WriteExpressionList(*iter);
                parameter_count += ((*iter)->GetChildren().size() + 1) / 2;
                WriteCall(class_name_ + "." + func_name, parameter_count);
            }
            break;
        }
        case TERMINAL_TOKEN_TYPE::SYMBOL: {
            auto symbol = (*iter)->GetContent();
            if (symbol == "(") {
                // expression
                ++iter;
                WriteExpression(*iter);
            } else {
                ++iter;
                auto kind = symbol_table_.KindOf(symbol);
                auto index = symbol_table_.IndexOf(symbol);
                WritePush(SymbolTableKindToSegmentType.at(kind), index);
                // unaryOp term
                if (symbol == "-") {
                    // "-"
                    WriteArithmetic(ArithmeticType::kNot);
                    WritePush(SegmentType::kConst, 1);
                    WriteArithmetic(ArithmeticType::kAdd);
                } else {
                    // "~"
                    WriteArithmetic(ArithmeticType::kNot);
                }
            }
            break;
        }
        default: {
            break;
        }
    }
}
}  // namespace jack_compiler
