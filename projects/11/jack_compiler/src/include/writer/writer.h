#ifndef JACK_COMPILER_WRITER_H_
#define JACK_COMPILER_WRITER_H_

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "writer/node.h"

namespace jack_compiler {
/**
 * The base class for all writer
 */
class Writer {
 public:
  explicit Writer(const std::string &file);
  explicit Writer(std::ostream *output);
  Writer(const Writer &) = delete;
  Writer(Writer &&) = delete;
  virtual ~Writer() = 0;
  /**
   * Write the whole content of root and its relatived nodes
   */
  void WriteToOutput(const std::shared_ptr<Node> &root);

 protected:
  virtual std::string Write(const std::shared_ptr<Node> &root, int level = 0) = 0;
  bool can_delete_output_;
  std::ostream *output_;
};
}  // namespace jack_compiler

#endif  // JACK_COMPILER_WRITER_H_
