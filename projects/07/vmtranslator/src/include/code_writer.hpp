#include <fstream>
#include <iostream>
#include <string>

#include "parser.hpp"

namespace vmtranslator {
    class CodeWriter {
        public:
            CodeWriter(std::ostream* output);
            CodeWriter(const std::string file);
            ~CodeWriter();

            void SetFileName(const std::string file_name);
            void WriteArithmetic(const std::string command);
            void WritePushpop(COMMAND command, std::string segment, int16_t index);
            void Close();
        private:
            // The output stream
            std::ostream* output_;
            std::string file_name_;
            bool can_delete_input_;
            // Record current address
            int16_t address_;
    };
} // namespace vmtranslator
