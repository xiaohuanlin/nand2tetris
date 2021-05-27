#include "parser.hpp"
#include "symbol_table.hpp"


int main(int argc, char* argv[]) {
    // support file only now
    if (argc < 2) {
        return 1;
    }

    char* input_file_name = argv[1];
    std::cout << "read file: " << input_file_name << std::endl;
    compiler::Parser build_parser(input_file_name), parser(input_file_name);
    compiler::Code code;
    compiler::SymbolTable symbol_table;

    std::string base_out_file_name(input_file_name);
    std::size_t pos = base_out_file_name.length();
    if ((pos = base_out_file_name.find_last_of('.')) != std::string::npos) {
        base_out_file_name.erase(pos);
    }
    std::string out_file_name = base_out_file_name + ".hack";
    if (argc > 2) {
       out_file_name = argv[2];
    }
    std::cout << "write file: " << out_file_name << std::endl;
    std::ofstream out_file_stream(out_file_name, std::ofstream::out);
    if (!out_file_stream.is_open()) {
        return 1;
    }

    // build symbol table
    int16_t address = 0;
    int16_t rom_address = 16;
    while (build_parser.HasMoreCommands()) {
        build_parser.Advance();
        
        switch (build_parser.GetCommandType()) {
            case compiler::COMMAND::A_COMMAND: {
                address += 1;
                break;
            }
            
            case compiler::COMMAND::L_COMMAND: {
                symbol_table.AddEntry(build_parser.GetSymbol(), address);
                break;
            }

            case compiler::COMMAND::C_COMMAND: {
                address += 1;
                break;
            }
            default:
                break;
        }
    }
    
    // parse
    while (parser.HasMoreCommands()) {
        parser.Advance();
        
        switch (parser.GetCommandType()) {
            case compiler::COMMAND::A_COMMAND: {
                std::string symbol_or_number = parser.GetSymbol();
                int16_t value;
                try {
                    // for number
                    value = std::stol(symbol_or_number);
                } catch (std::invalid_argument) {
                    if (symbol_table.Contains(symbol_or_number)) {
                        // for symbol
                        value = symbol_table.GetAddress(symbol_or_number);
                    } else {
                        // for variable
                        symbol_table.AddEntry(symbol_or_number, rom_address);
                        value = rom_address++;
                    }
                }
                out_file_stream << std::bitset<16>(value) << std::endl;
                break;
            }
            
            case compiler::COMMAND::L_COMMAND:
                break;

            case compiler::COMMAND::C_COMMAND: {
                out_file_stream << std::bitset<3>(0b111)
                                << code.Comp(parser.GetComp())
                                << code.Dest(parser.GetDest())
                                << code.Jump(parser.GetJump()) << std::endl;
                break;
            }

            default:
                break;
        }
    }
    out_file_stream.flush();
    out_file_stream.close();
}