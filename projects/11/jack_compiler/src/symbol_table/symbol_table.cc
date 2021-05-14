#include "symbol_table/symbol_table.h"

#include "exception/exception.h"

namespace jack_compiler {
    SymbolTable::SymbolTable() {}

    SymbolTable::~SymbolTable() {}

    void SymbolTable::StartSubroutine() {
        var_count_ = 0;
        argument_count_ = 0;
        subroutine_table_.clear();
    }

    void SymbolTable::Define(const Name &name, const Type &type, const SymbolTableKind &kind) {
        switch (kind) {
            case SymbolTableKind::kStatic: {
                class_table_.insert({name, {type, kind, static_count_++}});
                break;
            }
            case SymbolTableKind::kField: {
                class_table_.insert({name, {type, kind, field_count_++}});
                break;
            }
            case SymbolTableKind::kArg: {
                subroutine_table_.insert({name, {type, kind, argument_count_++}});
                break;
            }
            case SymbolTableKind::kVar: {
                subroutine_table_.insert({name, {type, kind, var_count_++}});
                break;
            }
            default: {
                break;
            }
        }
    }

    uint16_t SymbolTable::VarCount(SymbolTableKind kind) {
        switch (kind) {
            case SymbolTableKind::kStatic: {
                return static_count_;
            }
            case SymbolTableKind::kField: {
                return field_count_;
            }
            case SymbolTableKind::kArg: {
                return argument_count_;
            }
            case SymbolTableKind::kVar: {
                return var_count_;
            }
            default: {
                return 0;
            }
        }
    }

    auto SymbolTable::SearchName(const SymbolTable::Name &name) {
        // Consider the locality
        auto iter = subroutine_table_.find(name);
        if (iter == subroutine_table_.end()) {
            iter = class_table_.find(name);
        }
        if (iter == class_table_.end()) {
            THROW_COMPILER_EXCEPT
        }
        return iter;
    }

    SymbolTableKind SymbolTable::KindOf(const Name &name) {
        auto iter = SearchName(name);
        auto& [type, symbol_table_kind, index] = iter->second;
        return symbol_table_kind;
    }

    SymbolTable::Type SymbolTable::TypeOf(const Name &name) {
        auto iter = SearchName(name);
        auto& [type, symbol_table_kind, index] = iter->second;
        return type;
    }

    SymbolTable::Index SymbolTable::IndexOf(const Name &name) {
        auto iter = SearchName(name);
        auto& [type, symbol_table_kind, index] = iter->second;
        return index;
    }
}  // namespace jack_compiler
