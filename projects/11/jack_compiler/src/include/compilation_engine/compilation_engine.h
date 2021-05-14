#ifndef JACK_COMPILER_COMPILATION_ENGINE_H_
#define JACK_COMPILER_COMPILATION_ENGINE_H_

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "jack_tokenizer/jack_tokenizer.h"
#include "writer/node.h"
#include "writer/writer.h"

namespace jack_compiler {

template<class WriterType>
class CompilationEngine {
 public:
  explicit CompilationEngine(std::istream *input, std::ostream *output);
  CompilationEngine(const std::string &input_file,
                    const std::string &output_file);
  CompilationEngine(const CompilationEngine &) = delete;
  CompilationEngine(CompilationEngine &&) = delete;
  ~CompilationEngine();

  /**
   * Compile whole class
   */
  void CompileClass();
  /**
   * Compile static or field declaration
   */
  void CompileClassVarDec(std::shared_ptr<Node> parent);
  void CompileSubroutine(std::shared_ptr<Node> parent);
  void CompileSubroutineBody(std::shared_ptr<Node> parent);
  /**
   * Execute one more step
   */
  void CompileParameterList(std::shared_ptr<Node> parent);
  void CompileVarDec(std::shared_ptr<Node> parent);
  /**
   * Compile statement except for "{}"
   * Execute one more step
   */
  void CompileStatements(std::shared_ptr<Node> parent);
  /**
   * Execute one more step
   */
  void CompileDo(std::shared_ptr<Node> parent);
  /**
   * Execute one more step
   */
  void CompileLet(std::shared_ptr<Node> parent);
  /**
   * Execute one more step
   */
  void CompileWhile(std::shared_ptr<Node> parent);
  /**
   * Execute one more step
   */
  void CompileReturn(std::shared_ptr<Node> parent);
  /**
   * Execute one more step
   */
  void CompileIf(std::shared_ptr<Node> parent);
  /**
   * Execute one more step
   */
  void CompileExpression(std::shared_ptr<Node> parent);
  /**
   * We should peek next word to distinguish among the variable, the array and
   * the subroution call Execute one more step
   */
  void CompileTerm(std::shared_ptr<Node> parent);
  /**
   * Execute one more step
   */
  void CompileExpressionList(std::shared_ptr<Node> parent);
  void CompileSubroutineCall(std::shared_ptr<Node> parent);

 private:
  void MoveToNext();
  bool CompileType(std::shared_ptr<Node> parent);
  /**
   * Compile the operation symbol
   */
  bool CompileOp(std::shared_ptr<Node> parent);
  /**
   * Compile the unary operation symbol
   */
  bool CompileUnaryOp(std::shared_ptr<Node> parent);

  /**
   * Only compile the limited keywords. Return false if we can't compile it
   */
  bool CompileKeyword(std::shared_ptr<Node> parent,
                      std::vector<KEYWORD_TYPE> limit_types);
  /**
   * Only compile the limited symbols. Return false if we can't compile it
   */
  bool CompileSymbol(std::shared_ptr<Node> parent,
                     std::vector<std::string> limit_symbols);
  /**
   * Compile identifiers. Return false if we can't compile it
   */
  bool CompileIdentifier(std::shared_ptr<Node> parent);
  bool CompileIntegerConst(std::shared_ptr<Node> parent);
  bool CompileStringConst(std::shared_ptr<Node> parent);
  JackTokenizer tokenizer_;
  WriterType *writer_;
  std::shared_ptr<Node> root_;
};
}  // namespace jack_compiler

#endif  // JACK_COMPILER_COMPILATION_ENGINE_H_
