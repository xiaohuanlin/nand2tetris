#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "utils/enum/enum.hpp"

namespace vmtranslator {
    class CodeWriter {
        public:
            CodeWriter(std::ostream* output);
            CodeWriter(const std::string& file);
            ~CodeWriter();

            /**
             * Tell the current translated file name
             */
            void SetFileName(const std::string& file_name);
            /**
             * Write init code
             */
            void WriteInit();
            void WriteArithmetic(const std::string& command);
            void WritePushpop(COMMAND command, const std::string& segment, int16_t index);
            void WriteLabel(const std::string& symbol);
            void WriteGoto(const std::string& symbol);
            void WriteIf(const std::string& symbol);
            void WriteCall(const std::string& function_name, int16_t num_args);
            void WriteReturn();
            void WriteFunction(const std::string& funtion_name, int16_t num_locals);
            void Close();
        private:
            /**
             * Write the code inline
             */
            void WriteInline(const std::string& line);
            void WriteInline(const std::string&& line);

            /**
             * Write the address
             */
            void WriteAddress(int16_t address);
            void WriteAddress(const std::string& address);
            void WriteAddress(const std::string&& address);
            // The output stream
            std::ostream* output_;
            std::string file_name_;
            bool can_delete_input_ = false;
            // Record current address
            int16_t address_ = 0;
            // Record call times
            int16_t call_times_ = 0;
    };
} // namespace vmtranslator
