#ifndef JACK_COMPILER_EXCEPTION_H_
#define JACK_COMPILER_EXCEPTION_H_

#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

namespace jack_compiler {
#define THROW_COMPILER_EXCEPT                                                  \
  {                                                                            \
    throw CompileException(std::string(__FILE__) + ":" +                       \
                           std::to_string(__LINE__));                          \
  }

class CompileException : public std::logic_error {
 public:
  explicit CompileException(const std::string &info)
      : std::logic_error(info), info_("Compiler error: " + info_) {}

  const char *what() const noexcept { return info_.c_str(); };

 protected:
  std::string info_;
};
}  // namespace jack_compiler

#endif  // JACK_COMPILER_EXCEPTION_H_
