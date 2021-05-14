#ifndef JACK_COMPILER_JACK_COMPILER_H_
#define JACK_COMPILER_JACK_COMPILER_H_

#include <string>
#include <vector>

namespace jack_compiler {
class JackCompiler {
 public:
  JackCompiler(const std::string& input_file_or_dir);
  JackCompiler(const JackCompiler &) = delete;
  JackCompiler(JackCompiler &&) = delete;
  ~JackCompiler()=default;

  /**
   * Analyse the file or directory
   */
  void Analyze(std::string writer_class);

 private:
  /**
   * Read all .jack files from directory or individual file
   */
  void ReadInput();
  const std::string file_or_dir_name_;
  std::vector<std::string> files_;
};
}  // namespace jack_compiler

#endif  // JACK_COMPILER_JACK_COMPILER_H_
