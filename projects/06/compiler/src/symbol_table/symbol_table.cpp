#include "symbol_table.hpp"

namespace compiler
{
    void SymbolTable::AddEntry(const std::string symbol, const int address) {
        store_[symbol] = address;
    }

    bool SymbolTable::Contains(const std::string symbol) {
        return store_.find(symbol) != store_.end();
    }

    int SymbolTable::GetAddress(const std::string symbol) {
        return store_[symbol];
    }
} // namespace complier
