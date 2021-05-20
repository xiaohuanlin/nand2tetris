#include "writer/vm/vm_writer.h"

namespace jack_compiler {
void VMWriter::WritePush(const SegmentType &seg, const int &index) {
    string_stream_ << "push " << SegmentTypeString.at(seg) << " " << index << std::endl;
}

void VMWriter::WritePop(const SegmentType &seg, const int &index) {
    string_stream_ << "pop " << SegmentTypeString.at(seg) << " " << index << std::endl;
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
    string_stream_ << "function " << name << " " << n_args << std::endl;
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
    class_name_ = (*iter)->GetContent();
    ++iter;
    ++iter;
    while ((*iter)->GetTokenType().non_terminal_token_ == NonTerminalTokenType::kClassVarDec) {
        WriteClassVarDec(*iter);
        ++iter;
    }

    while ((*iter)->GetTokenType().non_terminal_token_ == NonTerminalTokenType::kSubroutineDec) {
        WriteClassSubroutineDec(*iter);
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
        ++iter;
    }
}

void VMWriter::WriteClassSubroutineDec(const std::shared_ptr<Node> &root) {
    symbol_table_.StartSubroutine();
    auto iter = root->GetChildren().begin();
    std::string function_type = (*iter)->GetContent();
    ++iter;
    ++iter;
    function_name_ = class_name_ + "." + (*iter)->GetContent();

    if (function_type == "method") {
        // define "this" before parameter to make sure the index of "this" is 0
        symbol_table_.Define("this", class_name_, SymbolTableKind::kArg);
    }

    ++iter;
    ++iter;
    WriteParameterList(*iter);

    ++iter;
    ++iter;
    int local_count = 0;
    for (auto &child: (*iter)->GetChildren()) {
        if (child->GetTokenType().non_terminal_token_ == NonTerminalTokenType::kVarDec) {
            local_count += (child->GetChildren().size() - 2) / 2;
        }
    }
    if (function_type == "constructor") {
        ++local_count;
    }

    WriteFunction(function_name_, local_count);
    if (function_type == "method") {
        WritePush(SegmentType::kArg, symbol_table_.IndexOf("this"));
        WritePop(SegmentType::kPointer, 0);
    } else if (function_type == "constructor") {
        // put "this" into table to make returning "this" possible
        symbol_table_.Define("this", class_name_, SymbolTableKind::kVar);
        WritePush(SegmentType::kConst, symbol_table_.VarCount(SymbolTableKind::kField));
        WriteCall("Memory.alloc", 1);
        WritePop(SegmentType::kLocal, symbol_table_.IndexOf("this"));
        // set "this" pointer
        WritePush(SegmentType::kLocal, symbol_table_.IndexOf("this"));
        WritePop(SegmentType::kPointer, 0);
    }
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
    while (iter != root->GetChildren().end() && (*iter)->GetContent() == ",") {
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
    while ((*iter)->GetTokenType().non_terminal_token_ == NonTerminalTokenType::kVarDec) {
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
            case NonTerminalTokenType::kLetStatement: {
                WriteLetStatement(node);
                break;
            }
            case NonTerminalTokenType::kDoStatement: {
                WriteDoStatement(node);
                break;
            }
            case NonTerminalTokenType::kWhileStatement: {
                WriteWhileStatement(node);
                break;
            }
            case NonTerminalTokenType::kIfStatement: {
                WriteIfStatement(node);
                break;
            }
            case NonTerminalTokenType::kReturnStatement: {
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
        ++iter;
        // store index iter
        auto index_iter = iter;
        ++iter;
        ++iter;
        ++iter;
        // calculate right value first
        WriteExpression(*iter);
        WritePush(SymbolTableKindToSegmentType.at(kind), index);
        WriteExpression(*index_iter);
        // calculate the postion of array[expression]
        WriteArithmetic(ArithmeticType::kAdd);
        // put it in "that" pointer
        WritePop(SegmentType::kPointer, 1);
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
        // className.subroutineName | varName.subroutineName
        // constructor or function | method
        ++iter;
        class_or_var_name = func_name;
        func_name = (*iter)->GetContent();
        ++iter;
    } else {
        // subroutineName
        // this.method
        class_or_var_name = "this";
    }

    bool is_class_name = false;
    try {
        // we should push "this" pointer into stack if it is a variable
        auto kind = symbol_table_.KindOf(class_or_var_name);
        auto index = symbol_table_.IndexOf(class_or_var_name);
        WritePush(SymbolTableKindToSegmentType.at(kind), index);
        ++parameter_count;
    } catch (...) {
        is_class_name = true;
    }

    ++iter;
    WriteExpressionList(*iter);
    parameter_count += ((*iter)->GetChildren().size() + 1) / 2;
    if (is_class_name) {
        WriteCall(class_or_var_name + "." + func_name, parameter_count);
    } else {
        auto type = symbol_table_.TypeOf(class_or_var_name);
        WriteCall(type + "." + func_name, parameter_count);
    }
    // pop and ignore the return value
    WritePop(SegmentType::kTemp, 0);
}

void VMWriter::WriteWhileStatement(const std::shared_ptr<Node> &root) {
    auto iter = root->GetChildren().begin();
    std::string count = std::to_string(label_count_++);
    WriteLabel(function_name_ + count + "START");
    ++iter;
    ++iter;
    WriteExpression(*iter);
    // expression == false?
    WritePush(SegmentType::kConst, 0);
    WriteArithmetic(ArithmeticType::kEq);
    ++iter;
    ++iter;
    ++iter;
    WriteIf(function_name_ + count + "NEXT");
    WriteStatements(*iter);
    WriteGoto(function_name_ + count + "START");
    WriteLabel(function_name_ + count + "NEXT");
}

void VMWriter::WriteIfStatement(const std::shared_ptr<Node> &root) {
    auto iter = root->GetChildren().begin();
    std::string count = std::to_string(label_count_++);

    ++iter;
    ++iter;
    WriteExpression(*iter);
    // expression == false?
    WritePush(SegmentType::kConst, 0);
    WriteArithmetic(ArithmeticType::kEq);
    ++iter;
    ++iter;
    ++iter;
    WriteIf(function_name_ + count + "NEXT");
    // true statements
    WriteStatements(*iter);
    ++iter;
    ++iter;
    WriteGoto(function_name_ + count + "REMAIN");
    WriteLabel(function_name_ + count + "NEXT");
    if (iter != root->GetChildren().end()) {
        // else part
        ++iter;
        ++iter;
        // false statements
        WriteStatements(*iter);
    }
    WriteLabel(function_name_ + count + "REMAIN");
}

void VMWriter::WriteReturnStatement(const std::shared_ptr<Node> &root) {
    auto iter = root->GetChildren().begin();
    ++iter;

    if ((*iter)->GetTokenType().non_terminal_token_ == NonTerminalTokenType::kExpression) {
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
    ++iter;
    while (iter != root->GetChildren().end()) {
        ++iter;
        WriteExpression(*iter);
        ++iter;
    }

}

void VMWriter::WriteExpression(const std::shared_ptr<Node> &root) {
    auto iter = root->GetChildren().begin();
    WriteTerm(*iter);
    ++iter;
    while (iter != root->GetChildren().end()) {
        OpType op = StringOpType.at((*iter)->GetContent());
        ++iter;
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
        case TerminalTokenType::kIntConst: {
            WritePush(SegmentType::kConst, std::stod((*iter)->GetContent()));
            break;
        }
        case TerminalTokenType::kStringConst: {
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
        case TerminalTokenType::kKeyword: {
            // for "true" "false" "null" "this"
            auto keyword = (*iter)->GetContent();
            if (keyword == "true") {
                WritePush(SegmentType::kConst, 1);
                WriteArithmetic(ArithmeticType::kNeg);
            } else if (keyword == "false") {
                WritePush(SegmentType::kConst, 0);
            } else if (keyword == "null") {
                WritePush(SegmentType::kConst, 0);
            } else if (keyword == "this") {
                WritePush(SegmentType::kPointer, 0);
            }
            break;
        }
        case TerminalTokenType::kIdentifier: {
            // varname | varname[expression] | subroutineCall
            auto varname = (*iter)->GetContent();
            ++iter;
            if (iter == root->GetChildren().end()) {
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
                std::string func_name = varname;
                int parameter_count = 0;

                if ((*iter)->GetContent() == ".") {
                    ++iter;
                    class_or_var_name = func_name;
                    func_name = (*iter)->GetContent();
                    ++iter;
                } else {
                    // subroutineName
                    // this.method
                    class_or_var_name = "this";
                }

                bool is_class_name = false;
                try {
                    // we should push "this" pointer into stack if it is a variable
                    auto kind = symbol_table_.KindOf(class_or_var_name);
                    auto index = symbol_table_.IndexOf(class_or_var_name);
                    WritePush(SymbolTableKindToSegmentType.at(kind), index);
                    ++parameter_count;
                } catch (...) {
                    is_class_name = true;
                }

                ++iter;
                WriteExpressionList(*iter);
                parameter_count += ((*iter)->GetChildren().size() + 1) / 2;
                if (is_class_name) {
                    WriteCall(class_or_var_name + "." + func_name, parameter_count);
                } else {
                    auto type = symbol_table_.TypeOf(class_or_var_name);
                    WriteCall(type + "." + func_name, parameter_count);
                }
            }
            break;
        }
        case TerminalTokenType::kSymbol: {
            auto symbol = (*iter)->GetContent();
            if (symbol == "(") {
                // expression
                ++iter;
                WriteExpression(*iter);
            } else {
                ++iter;
                WriteTerm(*iter);
                // unaryOp term
                if (symbol == "-") {
                    // "-"
                    WriteArithmetic(ArithmeticType::kNeg);
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
