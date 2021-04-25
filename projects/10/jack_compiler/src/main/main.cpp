#include <iostream>
#include <string>
#include <vector>

#include "jack_analyzer/jack_analyzer.hpp"
#include "main/main.hpp"
#include "utils/utils.hpp"

namespace jack_compiler {
    int Main::Run(int argc, char* argv[]) {
        if (argc < 3) {
            std::cout << "Need input file name" << std::endl;
            return 1;
        }

        const char *function = argv[1];
        bool to_token = std::string(function) == "token";

        JackAnalyzer analyzer(argv[2]);
        analyzer.Analyze(to_token);
        return 0;
    }
} // namespace jack_compiler
