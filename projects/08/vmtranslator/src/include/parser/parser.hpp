#pragma once

#include <fstream>
#include <iostream>
#include <tuple>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "utils/enum/enum.hpp"

namespace vmtranslator {

    class Parser {
        public:
            Parser(std::istream* input);
            Parser(const std::string file);
            ~Parser();

            /**
             * If there are more commands?
             */
            bool HasMoreCommands();

            /**
             * Only if there are more commands, we can use this function to next command 
             */
            void Advance();
            
            inline COMMAND GetCommandType() noexcept {return command_type_;};
            inline std::string GetArg1() noexcept {return arg1_;};
            inline int16_t GetArg2() noexcept {return arg2_;};
        private:
            /**
             * Judge if the read line is a valid command
             */
            bool IsValidCommand();
            /**
             * Judge if the read line is a arithmetic command
             */
            bool IsValidArithmeticCommand();
            /**
             * Judge if the read line is a memory access command
             */
            bool IsValidMemoryAccessCommand();
            /**
             * Judge if the read line is a control flow command
             */
            bool IsValidControlFlowCommand();
            /**
             * Judge if the read line is a function call command
             */
            bool IsValidFunctionCallCommand();

            /**
             * Judge if the symbol is valid 
             */
            bool IsValidSymbol(const std::string symbol);

            /**
             * Split the command into command_type, arg1, arg2
             */
            bool ParseCommand(const std::string& command); 

            // The input stream
            std::istream* input_;

            // Current command string buffer
            char current_command_[BUFFER_SIZE];


            // Code using for checking C command
            bool can_delete_input_;
            COMMAND command_type_;
            // Current command: arg1 
            std::string arg1_;
            // Current command: arg2 
            int16_t arg2_;
    };
} // namespace vmtranslator
