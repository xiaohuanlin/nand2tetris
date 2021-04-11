#include <vector>
#include <unordered_map>

#include "code_writer/code_writer.hpp"

namespace vmtranslator {
    CodeWriter::CodeWriter(std::ostream* output): output_(output) {};
    CodeWriter::CodeWriter(const std::string& file): output_(nullptr), can_delete_input_(true) {
        std::ofstream* open_file_p = new std::ofstream(file);
        if (!open_file_p->is_open()) {
            delete open_file_p;
            std::cerr << "Output file can't be opend" << std::endl;
            throw std::exception();
        }
        can_delete_input_ = true;
        output_ = open_file_p;
    }

    CodeWriter::~CodeWriter() {
        Close();
    };

    void CodeWriter::WriteInline(const std::string& line) {
        (*output_) << line << std::endl;
        ++address_;
    }

    void CodeWriter::WriteInline(const std::string&& line) {
        // std::cout << line << " use &&" << std::endl;
        WriteInline(line);
    }

    void CodeWriter::WriteAddress(int16_t address) {
        WriteInline("@" + std::to_string(address));
    }

    void CodeWriter::WriteAddress(const std::string& address) {
        WriteInline("@" + address);
    }

    void CodeWriter::WriteAddress(const std::string&& address) {
        WriteInline("@" + address);
    }

    void CodeWriter::SetFileName(const std::string& file_name) {
        // format file name
        size_t last_slash = file_name.find_last_of('/');
        if (last_slash == std::string::npos) {
            last_slash = -1;
        }
        size_t last_dot = file_name.find_last_of('.');
        if (last_dot == std::string::npos) {
            last_dot = file_name.length();
        }
        file_name_ = std::string(file_name, last_slash + 1, last_dot - last_slash - 1);
    };

    void CodeWriter::WriteInit() {
        WriteInline(SP_INIT_ADDRESS);
        WriteInline("D=A");
        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
        WriteInline("M=D");

        WriteCall("Sys.init", 0);
    };

    void CodeWriter::WriteArithmetic(const std::string& command) {
        ARITHMETIC_TYPE command_type = STRING_ARITHMETIC_TYPE.at(command);
        if (command_type == ARITHMETIC_TYPE::NEG || command_type == ARITHMETIC_TYPE::NOT) {
            // unary command
            WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
            WriteInline("A=M-1");
            if (command_type == ARITHMETIC_TYPE::NOT) {
                WriteInline("M=!M");
            } else {
                WriteInline("M=-M");
            }
        } else {
            // binary command
            WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
            // push y to D register
            WriteInline("A=M-1");
            WriteInline("D=M");
            // change sp to sp - 2 
            WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
            // push x address to A register
            WriteInline("M=M-1");
            WriteInline("A=M-1");

            if (command_type == ARITHMETIC_TYPE::ADD) {
                WriteInline("M=M+D");
            } else if (command_type == ARITHMETIC_TYPE::SUB) {
                WriteInline("M=M-D");
            } else if (command_type == ARITHMETIC_TYPE::EQ
                    || command_type == ARITHMETIC_TYPE::GT
                    || command_type == ARITHMETIC_TYPE::LT
                    ) {
                std::string jump_command = "J";
                if (command_type == ARITHMETIC_TYPE::EQ) {
                    jump_command += "EQ";
                } else if (command_type == ARITHMETIC_TYPE::GT) {
                    jump_command += "GT";
                } else {
                    jump_command += "LT";
                }

                WriteInline("D=M-D");
                WriteAddress(address_ + 7);
                WriteInline("D;" + jump_command);
                // not compare 0
                WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
                WriteInline("A=M-1");
                WriteInline("M=0");
                WriteAddress(address_ + 5);
                WriteInline("0;JMP");
                // compare 0
                WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
                WriteInline("A=M-1");
                WriteInline("M=-1");
            } else if (command_type == ARITHMETIC_TYPE::AND) {
                WriteInline("M=D&M");
            } else if (command_type == ARITHMETIC_TYPE::OR) {
                WriteInline("M=D|M");
            }

        }
    };

    void CodeWriter::WritePushpop(COMMAND command, const std::string& segment, int16_t index) {
        MEMORY_AREA area = STRING_MEMORY_AREA.at(segment);

        switch (command) {
            case COMMAND::C_PUSH: {
                if (area == MEMORY_AREA::CONSTANT) {
                    // for constant
                    WriteAddress(index);
                    WriteInline("D=A");
                    WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
                    WriteInline("A=M");
                    WriteInline("M=D");
                    WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
                    WriteInline("M=M+1");
                } else if (area == MEMORY_AREA::TEMP || area == MEMORY_AREA::POINTER) {
                    WriteAddress(SEGMENT_BEGIN_ADDRESS.at(area) + index);
                    WriteInline("D=M");

                    WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
                    WriteInline("A=M");
                    WriteInline("M=D");
                    WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
                    WriteInline("M=M+1");
                } else if (area == MEMORY_AREA::STATIC) {
                        WriteAddress(file_name_ + "." + std::to_string(index));
                        WriteInline("D=M");
                        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
                        WriteInline("A=M");
                        WriteInline("M=D");
                        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
                        WriteInline("M=M+1");
                } else {
                    // for local, argument, this, that
                    WriteAddress(index);
                    WriteInline("D=A");
                    WriteAddress(SEGMENT_BEGIN_ADDRESS.at(area));
                    WriteInline("A=D+M");
                    WriteInline("D=M");
                    WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
                    WriteInline("A=M");
                    WriteInline("M=D");
                    WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
                    WriteInline("M=M+1");
                }
                break;
            }
            case COMMAND::C_POP: {
                if (area == MEMORY_AREA::TEMP || area == MEMORY_AREA::POINTER) {
                    WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
                    WriteInline("AM=M-1");
                    WriteInline("D=M");

                    WriteAddress(SEGMENT_BEGIN_ADDRESS.at(area) + index);
                    WriteInline("M=D");
                } else if (area == MEMORY_AREA::STATIC) {
                    WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
                    WriteInline("AM=M-1");
                    WriteInline("D=M");
                    WriteAddress(file_name_ + "." + std::to_string(index));
                    WriteInline("M=D");
                } else {
                    // for local, argument, this, that
                    // GENERAL = *area + index
                    WriteAddress(index);
                    WriteInline("D=A");
                    WriteAddress(SEGMENT_BEGIN_ADDRESS.at(area));
                    WriteInline("D=D+M");
                    WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::GENERAL_USE_1));
                    WriteInline("M=D");
                    
                    // D = pop()
                    WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
                    WriteInline("AM=M-1");
                    WriteInline("D=M");
                    // *GENERAL = D
                    WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::GENERAL_USE_1));
                    WriteInline("A=M");
                    WriteInline("M=D");
                }
                break;
            }
            default:
                break;
        }
    }

    void CodeWriter::WriteLabel(const std::string& symbol) {
        (*output_)  << "(" << symbol << ")" << std::endl;
    }

    void CodeWriter::WriteGoto(const std::string& symbol) {
        WriteAddress(symbol);
        WriteInline("0;JMP");
    }

    void CodeWriter::WriteIf(const std::string& symbol) {
        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
        WriteInline("AM=M-1");
        WriteInline("D=M");
        WriteAddress(symbol);
        WriteInline("D;JNE");
    }

    void CodeWriter::WriteCall(const std::string& function_name, int16_t num_args) {
        // push return-address
        call_times_++;
        std::string return_address = function_name + "." + std::to_string(call_times_);
        WriteAddress(return_address);
        WriteInline("D=A");
        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
        WriteInline("A=M");
        WriteInline("M=D");
        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
        WriteInline("M=M+1");

        static std::array<MEMORY_AREA, 4> need_save_area{
            MEMORY_AREA::LOCAL,
            MEMORY_AREA::ARGUMENT,
            MEMORY_AREA::THIS,
            MEMORY_AREA::THAT
        };

        for (auto& area: need_save_area) {
            // push LCL ARG THIS THAT 
            WriteAddress(SEGMENT_BEGIN_ADDRESS.at(area));
            WriteInline("D=M");
            WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
            WriteInline("A=M");
            WriteInline("M=D");
            WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
            WriteInline("M=M+1");
        }

        // ARG = SP - n - 5
        WriteAddress(num_args + 5);
        WriteInline("D=A");
        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
        WriteInline("D=M-D");
        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::ARGUMENT));
        WriteInline("M=D");
        // LCL = SP
        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
        WriteInline("D=M");
        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::LOCAL));
        WriteInline("M=D");
        // goto f
        WriteAddress(function_name);
        WriteInline("0;JMP");
        // (return-address)
        WriteLabel(return_address);
    }

    void CodeWriter::WriteReturn() {
        // FRAME = LCL
        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::LOCAL));
        WriteInline("D=M");
        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::GENERAL_USE_1));
        WriteInline("M=D");
        // RET = *(FRAME-5)
        WriteAddress(5);
        WriteInline("A=D-A");
        WriteInline("D=M");
        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::GENERAL_USE_2));
        WriteInline("M=D");
        // *ARG = *(SP - 1)
        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
        WriteInline("A=M-1");
        WriteInline("D=M");
        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::ARGUMENT));
        WriteInline("A=M");
        WriteInline("M=D");
        // SP = ARG + 1 
        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::ARGUMENT));
        WriteInline("D=M+1");
        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::SP));
        WriteInline("M=D");

        static std::array<MEMORY_AREA, 4> need_restore_area{
            MEMORY_AREA::THAT,
            MEMORY_AREA::THIS,
            MEMORY_AREA::ARGUMENT,
            MEMORY_AREA::LOCAL,
        };

        int16_t i = 0;
        for (auto& area: need_restore_area) {
            // THAT THIS ARG LCL = *(FRAME - i) 
            WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::GENERAL_USE_1));
            WriteInline("D=M");
            WriteAddress(++i);
            WriteInline("A=D-A");
            WriteInline("D=M");
            WriteAddress(SEGMENT_BEGIN_ADDRESS.at(area));
            WriteInline("M=D");
        }

        // goto RET
        WriteAddress(SEGMENT_BEGIN_ADDRESS.at(MEMORY_AREA::GENERAL_USE_2));
        WriteInline("A=M");
        WriteInline("0;JMP");
    }

    void CodeWriter::WriteFunction(const std::string& function_name, int16_t num_locals) {
        WriteLabel(function_name);
        for (int i = 0; i < num_locals; ++i) {
            WritePushpop(COMMAND::C_PUSH, "constant", 0);
        }
    }

    void CodeWriter::Close() {
        if (can_delete_input_) {
            delete reinterpret_cast<std::ofstream*>(output_);
            can_delete_input_ = false;
        }
    }
} // namespace vmtranslator
