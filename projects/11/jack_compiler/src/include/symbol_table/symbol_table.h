#ifndef JACK_COMPILER_SYMBOL_TABLE_H_
#define JACK_COMPILER_SYMBOL_TABLE_H_

#include <tuple>
#include <string>
#include <unordered_map>
#include <vector>

#include "utils/enum/enum.h"

namespace jack_compiler {
class SymbolTable {
 public:
  using Index = int;
  using Name = std::string;
  using Type = std::string;
  using Tuple = std::tuple<Type, SymbolTableKind, Index>;
  using Table = std::unordered_map<Name, Tuple>;

  SymbolTable();
  SymbolTable(const SymbolTable &) = delete;
  SymbolTable(SymbolTable &&) = delete;
  ~SymbolTable();

  /**
   * Start a new subroutine table
   */
  void StartSubroutine();
  void Define(const Name &name, const Type &type, const SymbolTableKind &kind);
  uint16_t VarCount(SymbolTableKind kind);
  /**
   * Return the kind of identifier
   */
  SymbolTableKind KindOf(const Name &name);
  /**
   * Return the type of identifier
   */
  Type TypeOf(const Name &name);
  /**
   * Return the index of identifier
   */
  Index IndexOf(const Name &name);
private:
  auto SearchName(const Name &name);
  uint16_t static_count_ = 0;
  uint16_t field_count_ = 0;
  uint16_t argument_count_ = 0;
  uint16_t var_count_ = 0;

  Table class_table_ = {};
  Table subroutine_table_ = {};
};
}  // namespace jack_compiler

#endif  // JACK_COMPILER_SYMBOL_TABLE_H_
