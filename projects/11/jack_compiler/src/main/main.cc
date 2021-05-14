#include <iostream>
#include <string>
#include <vector>

#include "jack_compiler/jack_compiler.h"
#include "main/main.h"
#include "utils/utils.h"

namespace jack_compiler {
int Main::Run(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Need input file name" << std::endl;
        return 1;
    }
    JackCompiler compiler(argv[1]);
    std::string writer_class = argv[2];
    compiler.Analyze(writer_class);
    return 0;
}
}  // namespace jack_compiler
