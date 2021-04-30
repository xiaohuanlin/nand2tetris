#ifndef JACK_COMPILER_SYMBOL_TABLE_H_
#define JACK_COMPILER_SYMBOL_TABLE_H_

#include <string>

namespace jack_compiler {
class SymbolTable {
 public:
  SymbolTable();
  SymbolTable(const SymbolTable &) = delete;
  SymbolTable(SymbolTable &&) = delete;
  ~SymbolTable();

  void StartSubroutine();
  void Define();
  int VarCount();
  void KindOf();
  std::string TypeOf();
  int IndexOf();
};
}  // namespace jack_compiler

#endif  // JACK_COMPILER_SYMBOL_TABLE_H_
