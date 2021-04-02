#include <iostream>
#include <string>
#include <fstream>

namespace compiler {
   class Parser {
        public:
            Parser(std::istream* input);
            Parser(const std::string file);
            ~Parser() = default;

            /**
             * If there are more commands?
             */
            bool HasMoreCommands();

            /**
             * Only if there are more commands, we can use this function to next command 
             */
            void Advance();
        private:
            std::istream* input_;
            int command_type_;
            // Current symbol or value
            std::string symbol_;
            // dest field of C command
            std::string dest_;
            // comp field of C command 
            std::string comp_;
            // jump field of C command
            std::string jump_;
   };
} // namespace compiler
