#ifndef JACK_COMPILER_VM_VMWRITER_H_
#define JACK_COMPILER_VM_VMWRITER_H_

#include <iostream>
#include <memory>
#include <string>
#include <sstream>

#include "utils/enum/enum.h"
#include "symbol_table/symbol_table.h"
#include "writer/writer.h"

namespace jack_compiler {
class VMWriter : public Writer {
 public:
  explicit VMWriter(const std::string &file) : Writer(file) {}
  explicit VMWriter(std::ostream *output) : Writer(output) {}
  VMWriter(const VMWriter &) = delete;
  VMWriter(VMWriter &&) = delete;
  virtual ~VMWriter() = default;

  void WritePush(const SegmentType &seg, const int &index);
  void WritePop(const SegmentType &seg, const int &index);
  void WriteArithmetic(const ArithmeticType &arithmetic_command);
  void WriteLabel(const std::string &label);
  void WriteGoto(const std::string &label);
  void WriteIf(const std::string &label);
  void WriteCall(const std::string &name, const int &n_args);
  void WriteFunction(const std::string &name, const int &n_args);
  void WriteReturn();

  std::string Write(const std::shared_ptr<Node> &root, __attribute__((unused)) int level = 0) final;
 protected:
  void WriteClass(const std::shared_ptr<Node> &root);
  void WriteClassVarDec(const std::shared_ptr<Node> &root);
  void WriteClassSubroutineDec(const std::shared_ptr<Node> &root);
  void WriteParameterList(const std::shared_ptr<Node> &root);
  void WriteSubroutineBody(const std::shared_ptr<Node> &root);
  void WriteSubroutineVarDec(const std::shared_ptr<Node> &root);
  void WriteStatements(const std::shared_ptr<Node> &root);
  void WriteLetStatement(const std::shared_ptr<Node> &root);
  void WriteDoStatement(const std::shared_ptr<Node> &root);
  void WriteWhileStatement(const std::shared_ptr<Node> &root);
  void WriteIfStatement(const std::shared_ptr<Node> &root);
  void WriteReturnStatement(const std::shared_ptr<Node> &root);
  void WriteExpressionList(const std::shared_ptr<Node> &root);
  void WriteExpression(const std::shared_ptr<Node> &root);
  /**
   * Push calculated value into stack
   */
  void WriteTerm(const std::shared_ptr<Node> &root);

  std::string class_name_;
  std::string function_name_;
  int label_count_ = 0;
  SymbolTable symbol_table_;
  std::stringstream string_stream_;
};
}  // namespace jack_compiler

#endif  // JACK_COMPILER_VM_VMWRITER_H_
