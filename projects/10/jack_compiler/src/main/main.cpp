#include <iostream>
#include <string>
#include <vector>

#include "jack_analyzer/jack_analyzer.hpp"
#include "main/main.hpp"
#include "utils/utils.hpp"

namespace jack_compiler {
    int Main::Run(int argc, char* argv[]) {
        if (argc < 2) {
            std::cout << "Need input file name" << std::endl;
            return 1;
        }
        JackAnalyzer analyzer(argv[1]);
        analyzer.Analyze();
    }
} // namespace jack_compiler
