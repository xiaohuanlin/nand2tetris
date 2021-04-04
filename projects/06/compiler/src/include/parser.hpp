#include <fstream>
#include <iostream>
#include <string>

#include "code.hpp"

namespace compiler {
    
    enum COMMAND {
        A_COMMAND,
        C_COMMAND,
        L_COMMAND,
    };

    const static std::streamsize BUFFER_SIZE = 256;

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
            
            inline int GetCommandType() noexcept {return command_type_;};
            inline std::string GetSymbol() noexcept {return symbol_;};
            inline std::string GetDest() noexcept {return dest_;};
            inline std::string GetComp() noexcept {return comp_;};
            inline std::string GetJump() noexcept {return jump_;};
        private:
            /**
             * Judge if the read line is a valid command
             */
            bool IsValidCommand();
            /**
             * Judge if the symbol meet the requirement 
             */
            bool IsValidSymbol(const std::string symbol);
            // The input stream
            std::istream* input_;
            // Current command string buffer
            char current_command_[BUFFER_SIZE];

            // Code using for checking C command
            Code* code_; 
            bool can_delete_input_;
            int command_type_;
            // Current symbol or value
            std::string symbol_;
            // The dest field of C command
            std::string dest_;
            // The comp field of C command 
            std::string comp_;
            // The jump field of C command
            std::string jump_;
    };
} // namespace compiler
