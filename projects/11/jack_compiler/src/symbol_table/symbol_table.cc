#include "symbol_table/symbol_table.h"

namespace jack_compiler {
    SymbolTable::SymbolTable() {}

    SymbolTable::~SymbolTable() {}

    void SymbolTable::StartSubroutine() {}
    void SymbolTable::Define() {}
    int SymbolTable::VarCount() {return 0;}
    void SymbolTable::KindOf() {}
    std::string SymbolTable::TypeOf() {return "";}
    int SymbolTable::IndexOf() {return 0;}
}  // namespace jack_compiler
