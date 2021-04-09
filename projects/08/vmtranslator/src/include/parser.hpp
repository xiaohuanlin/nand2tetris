#include <fstream>
#include <iostream>
#include <tuple>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace vmtranslator {
    
    enum class COMMAND {
        C_ARITHMETIC,
        C_PUSH,
        C_POP,
        C_LABEL,
        C_GOTO,
        C_IF,
        C_FUNCTION,
        C_RETURN,
        C_CALL,
    };

    enum class MEMORY_COMMAND {
        ARGUMENT,
        LOCAL,
        STATIC,
        CONSTANT,
        THIS,
        THAT,
        POINTER,
        TEMP
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
            bool IsValidArithmeticCommand(const std::string& command);
            /**
             * Judge if the read line is a memory access command
             */
            bool IsValidMemoryAccessCommand(const std::string& command);
            /**
             * Judge if the read line is a control flow command
             */
            bool IsValidControlFlowCommand(const std::string& command);
            /**
             * Judge if the read line is a function call command
             */
            bool IsValidFunctionCallCommand(const std::string& command);

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
            // Valid arithmetic commands
            std::unordered_set<std::string> arithmetic_commands_ {
                "add",
                "sub",
                "neg",
                "eq",
                "gt",
                "lt",
                "and",
                "or",
                "not",
            };

            std::unordered_map<std::string, COMMAND> commands_form_ {
                {"add", COMMAND::C_ARITHMETIC},
                {"sub", COMMAND::C_ARITHMETIC},
                {"neg", COMMAND::C_ARITHMETIC},
                {"eq", COMMAND::C_ARITHMETIC},
                {"gt", COMMAND::C_ARITHMETIC},
                {"lt", COMMAND::C_ARITHMETIC},
                {"and", COMMAND::C_ARITHMETIC},
                {"or", COMMAND::C_ARITHMETIC},
                {"not", COMMAND::C_ARITHMETIC},
                {"push", COMMAND::C_PUSH},
                {"pop", COMMAND::C_POP},
                {"if-goto", COMMAND::C_IF},
                {"goto", COMMAND::C_GOTO},
                {"label", COMMAND::C_LABEL},
                {"function", COMMAND::C_FUNCTION},
                {"return", COMMAND::C_RETURN},
                {"call", COMMAND::C_CALL},
            };

            // Code using for checking C command
            bool can_delete_input_;
            COMMAND command_type_;
            // Current command: arg1 
            std::string arg1_;
            // Current command: arg2 
            int16_t arg2_;
    };
} // namespace vmtranslator
