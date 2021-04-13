#pragma once

#include <vector>
#include <string>

namespace jack_compiler {
    class JackAnalyzer {
    public:
        JackAnalyzer(const std::string input_file_or_dir);

        /**
         * Analyse the file or directory
         */
        void Analyze();
    private:
        /**
         * Read all .jack files from directory or individual file
         */
        void ReadInput();
        std::string file_or_dir_name_;
        std::vector<std::string> files_;
    };
}