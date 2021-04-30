#ifndef JACK_COMPILER_JACK_ANALYZER_H_
#define JACK_COMPILER_JACK_ANALYZER_H_

#include <string>
#include <vector>

namespace jack_compiler {
class JackAnalyzer {
 public:
  explicit JackAnalyzer(const std::string &input_file_or_dir);
  JackAnalyzer(const JackAnalyzer &) = delete;
  JackAnalyzer(JackAnalyzer &&) = delete;
  ~JackAnalyzer() = default;

  /**
   * Analyse the file or directory
   */
  void Analyze(const bool &to_token);

 private:
  /**
   * Read all .jack files from directory or individual file
   */
  void ReadInput();
  const std::string file_or_dir_name_;
  std::vector<std::string> files_;
};
}  // namespace jack_compiler

#endif  // JACK_COMPILER_JACK_ANALYZER_H_
