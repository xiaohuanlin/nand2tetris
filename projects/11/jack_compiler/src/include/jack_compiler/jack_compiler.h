#ifndef JACK_COMPILER_JACK_COMPILER_H_
#define JACK_COMPILER_JACK_COMPILER_H_

namespace jack_compiler {
class JackCompiler {
 public:
  JackCompiler();
  JackCompiler(const JackCompiler &) = delete;
  JackCompiler(JackCompiler &&) = delete;
  ~JackCompiler();
};
}  // namespace jack_compiler

#endif  // JACK_COMPILER_JACK_COMPILER_H_
