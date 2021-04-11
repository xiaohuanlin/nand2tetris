#include <iostream>
#include <string>
#include <vector>

#include "code_writer/code_writer.hpp"
#include "main/main.hpp"
#include "parser/parser.hpp"
#include "utils/utils.hpp"

namespace vmtranslator
{
    Main::Main(bool write_init): write_init_(write_init) {};

    int Main::Run(int argc, char* argv[]) {
        if (argc < 2) {
            std::cout << "Need input file name" << std::endl;
            return 1;
        }
        if (argc < 3) {
            std::cout << "Need specify the output file name" << std::endl;
            return 1;
        }

        char* input_file_name_or_dir = argv[1];
        std::vector<std::string> filenames;

        // read all files
        Utils::ReadDir(input_file_name_or_dir, filenames);
        if (filenames.empty()) {
            std::cout << "empty file or dir" << std::endl;
            return 1;
        }

        if (filenames.size() > 1 && argc < 3) {
            std::cout << "Need specify the output file name" << std::endl;
            return 1;
        }

        // set output file name
        std::string base_out_file_name = argv[2];
        std::size_t pos = base_out_file_name.length();
        if ((pos = base_out_file_name.find_last_of('.')) != std::string::npos) {
            base_out_file_name.erase(pos);
        }
        std::string out_file_name = base_out_file_name + ".asm";

        std::cout << "write file: " << out_file_name << std::endl;
        vmtranslator::CodeWriter code_writer(out_file_name);

        if (write_init_) {
            code_writer.WriteInit();
        }

        for (auto& file_name: filenames) {
            code_writer.SetFileName(file_name);
            vmtranslator::Parser parser(file_name);

            // parse
            while (parser.HasMoreCommands()) {
                parser.Advance();
                
                switch (parser.GetCommandType()) {
                    case vmtranslator::COMMAND::C_ARITHMETIC: {
                        code_writer.WriteArithmetic(parser.GetArg1());
                        break;
                    }
                    case vmtranslator::COMMAND::C_POP:
                    case vmtranslator::COMMAND::C_PUSH: {
                        code_writer.WritePushpop(parser.GetCommandType(), parser.GetArg1(), parser.GetArg2());
                        break;
                    }
                    case vmtranslator::COMMAND::C_IF: {
                        code_writer.WriteIf(parser.GetArg1());
                        break;
                    }
                    case vmtranslator::COMMAND::C_GOTO: {
                        code_writer.WriteGoto(parser.GetArg1());
                        break;
                    }
                    case vmtranslator::COMMAND::C_LABEL: {
                        code_writer.WriteLabel(parser.GetArg1());
                        break;
                    }
                    case vmtranslator::COMMAND::C_FUNCTION: {
                        code_writer.WriteFunction(parser.GetArg1(), parser.GetArg2());
                        break;
                    }
                    case vmtranslator::COMMAND::C_RETURN: {
                        code_writer.WriteReturn();
                        break;
                    }
                    case vmtranslator::COMMAND::C_CALL: {
                        code_writer.WriteCall(parser.GetArg1(), parser.GetArg2());
                        break;
                    }
                    default:
                        throw std::invalid_argument("Unknown command");
                }
            }
        }
        code_writer.Close();
        return 0;
    }
} // namespace vmtranslator
