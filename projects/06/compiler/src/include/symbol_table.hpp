#include <string>

namespace compiler
{
    class SymbolTable
    {
    private:
    public:
        SymbolTable();
        SymbolTable();
        /**
         * Add (symbol, address) to table
         */
        void AddEntry(std::string symbol, int address);
        /**
         * Does there exist this symbol?
         */
        bool Contains(std::string symbol);
        /**
         * Get the address related with the symbol
         */
        int GetAddress(std::string symbol);
    };
} // namespace compiler
