#ifndef JACK_COMPILER_UTILS_H_
#define JACK_COMPILER_UTILS_H_

#include <string>
#include <vector>

namespace jack_compiler {
namespace utils {
/**
 * Retrieve files from directory recursively and store those filename to files
 */
void ReadDir(const std::string &dir_or_file, std::vector<std::string> *files);
/**
 * Store .vm file to files
 */
void StoreVMFile(const std::string &file, std::vector<std::string> *files);
}  // namespace utils
}  // namespace jack_compiler

#endif  // JACK_COMPILER_UTILS_H_
