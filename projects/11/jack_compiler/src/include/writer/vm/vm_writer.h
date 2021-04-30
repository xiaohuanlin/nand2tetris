#ifndef JACK_COMPILER_VM_VMWRITER_H_
#define JACK_COMPILER_VM_VMWRITER_H_

#include <memory>
#include <string>

#include "writer/writer.h"

namespace jack_compiler {
class VMWriter : public Writer {
 public:
  explicit VMWriter(const std::string &file) : Writer(file) {}
  explicit VMWriter(std::ostream *output) : Writer(output) {}
  VMWriter(const VMWriter &) = delete;
  VMWriter(VMWriter &&) = delete;
  virtual ~VMWriter() = default;

  void WritePush();
  void WritePop();
  void WriteArithmetic();
  void WriteLabel();
  void WriteGoto();
  void WriteIf();
  void WriteCall();
  void WriteFunction();
  void WriteReturn();
  void Close();

 private:
  std::string Write(const std::shared_ptr<Node> &root, int level = 0) final;
};
}  // namespace jack_compiler

#endif  // JACK_COMPILER_VM_VMWRITER_H_
