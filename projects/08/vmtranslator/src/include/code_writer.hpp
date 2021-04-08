#include <algorithm>
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
            /**
             * Write init code
             */
            void WriteInit();
            void WriteArithmetic(const std::string command);
            void WritePushpop(COMMAND command, std::string segment, int16_t index);
            void WriteLabel(const std::string symbol);
            void WriteGoto(const std::string symbol);
            void WriteIf(const std::string symbol);
            void WriteCall(const std::string function_name, const int num_args);
            void WriteReturn();
            void WriteFunction(const std::string funtion_name, const int num_locals);
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
