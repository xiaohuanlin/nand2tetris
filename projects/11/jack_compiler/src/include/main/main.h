#ifndef JACK_COMPILER_MAIN_H_
#define JACK_COMPILER_MAIN_H_

namespace jack_compiler {
class Main {
 public:
  Main() = default;
  Main(const Main &) = delete;
  Main(Main &&) = delete;
  ~Main() = default;
  /**
   * The main function for whole project
   */
  int Run(int argc, char *argv[]);
};
}  // namespace jack_compiler

#endif  // JACK_COMPILER_MAIN_H_
