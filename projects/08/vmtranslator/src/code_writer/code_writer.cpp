#include <vector>

#include "code_writer.hpp"

namespace vmtranslator {
    CodeWriter::CodeWriter(std::ostream* output): output_(output), can_delete_input_(false), address_(0) {
    }
    CodeWriter::CodeWriter(const std::string file): output_(nullptr), can_delete_input_(true), address_(0) {
        std::ofstream* open_file_p = new std::ofstream(file);
        if (!open_file_p->is_open()) {
            delete open_file_p;
            throw std::exception();
        }
        can_delete_input_ = true;
        output_ = open_file_p;
    }

    CodeWriter::~CodeWriter() {
        Close();
    };

    void CodeWriter::SetFileName(const std::string file_name) {
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
        (*output_)  << "@256"   << std::endl
                    << "D=A"    << std::endl
                    << "@0"     << std::endl
                    << "M=D"    << std::endl;
        address_ += 4;

        WriteCall("Sys.init", 0);
    };

    void CodeWriter::WriteArithmetic(const std::string command) {
        if (command == "neg" || command == "not") {
            // unary command
            (*output_)  << "@0"    << std::endl
                        << "A=M-1"  << std::endl;
            if (command == "not") {
                (*output_)  << "M=!M"   << std::endl;
            } else {
                (*output_)  << "M=-M"   << std::endl;
            }
            address_ += 3;
        } else {
            // binary command
            (*output_)  << "@0"    << std::endl
                        // push y to D register
                        << "A=M-1"  << std::endl
                        << "D=M"    << std::endl
                        // change sp to sp - 2 
                        << "@0"    << std::endl
                        // push x address to A register
                        << "M=M-1" << std::endl
                        << "A=M-1" << std::endl;
            address_ += 6;
            if (command == "add") {
                (*output_)  << "M=M+D"  << std::endl;
                address_ += 1;
            } else if (command == "sub") {
                (*output_)  << "M=M-D"  << std::endl;
                address_ += 1;
            } else if (command == "eq" || command == "gt" || command == "lt") {
                std::string jump_command = "J";
                if (command == "eq") {
                    jump_command += "EQ";
                } else if (command == "gt") {
                    jump_command += "GT";
                } else {
                    jump_command += "LT";
                }

                (*output_)  << "D=M-D"      << std::endl
                            << "@" << address_ + 8 << std::endl
                            << "D;" << jump_command << std::endl
                            // not compare 0
                            << "@0"         << std::endl
                            << "A=M-1"      << std::endl
                            << "M=0"        << std::endl
                            << "@" << address_ + 11 << std::endl
                            << "0;JMP"      << std::endl
                            // compare 0
                            << "@0"         << std::endl
                            << "A=M-1"      << std::endl
                            << "M=-1"       << std::endl;
                address_ += 11;
            } else if (command == "and") {
                (*output_)  << "M=D&M"  << std::endl;
                address_ += 1;
            } else if (command == "or") {
                (*output_)  << "M=D|M"  << std::endl;
                address_ += 1;
            }

        }
    };

    void CodeWriter::WritePushpop(COMMAND command, std::string segment, int16_t index) {
        static std::unordered_map<std::string, int> shift_segment_pos = {
            {"pointer",3},
            {"temp",   5},
        };
        static std::unordered_map<std::string, int> segment_pos = {
            {"local",   1},
            {"argument",2},
            {"this",    3},
            {"that",    4},
        };
        switch (command) {
            case COMMAND::C_PUSH: {
                if (segment == "constant") {
                    // for constant
                    (*output_)  << "@" << index << std::endl
                                << "D=A"    << std::endl
                                << "@0"     << std::endl
                                << "A=M"    << std::endl
                                << "M=D"    << std::endl
                                << "@0"     << std::endl
                                << "M=M+1"  << std::endl;
                    address_ += 7;
                } else if (segment == "temp" || segment == "pointer") {
                    auto iter = shift_segment_pos.find(segment);
                    if (iter != shift_segment_pos.end()) {
                        int pos = iter->second;
                        (*output_)  << "@" << pos + index << std::endl
                                    << "D=M"    << std::endl
                                    << "@0"     << std::endl
                                    << "A=M"    << std::endl
                                    << "M=D"    << std::endl
                                    << "@0"     << std::endl
                                    << "M=M+1"  << std::endl;
                        address_ += 7;
                    }
                } else if (segment == "static") {
                    (*output_)  << "@" << file_name_ << "." << index << std::endl
                                << "D=M"    << std::endl
                                << "@0"     << std::endl
                                << "A=M"    << std::endl
                                << "M=D"    << std::endl
                                << "@0"     << std::endl
                                << "M=M+1"  << std::endl;
                    address_ += 7;

                } else {
                    // for local, argument, this, that
                    auto iter = segment_pos.find(segment);
                    if (iter != segment_pos.end()) {
                        int pos = iter->second;
                        (*output_)  << "@" << index << std::endl
                                    << "D=A"    << std::endl
                                    << "@" << pos << std::endl
                                    << "A=D+M"  << std::endl
                                    << "D=M"    << std::endl
                                    << "@0"     << std::endl
                                    << "A=M"    << std::endl
                                    << "M=D"    << std::endl
                                    << "@0"     << std::endl
                                    << "M=M+1"  << std::endl;
                        address_ += 10;
                    }
                }
                break;
            }
            case COMMAND::C_POP: {
                if (segment == "temp" || segment == "pointer") {
                    auto iter = shift_segment_pos.find(segment);
                    if (iter != shift_segment_pos.end()) {
                        int pos = iter->second;
                        (*output_)  << "@0"     << std::endl
                                    << "AM=M-1" << std::endl
                                    << "D=M"    << std::endl
                                    << "@" << index + pos << std::endl
                                    << "M=D"    << std::endl;
                        address_ += 5;
                    }
                } else if (segment == "static") {
                    (*output_)  << "@0"     << std::endl
                                << "AM=M-1" << std::endl
                                << "D=M"    << std::endl
                                << "@" << file_name_ << "." << index << std::endl
                                << "M=D"    << std::endl;
                    address_ += 5;
                } else {
                    // for local, argument, this, that
                    auto iter = segment_pos.find(segment);
                    if (iter != segment_pos.end()) {
                        int pos = iter->second;
                        (*output_)  << "@" << index << std::endl
                                    << "D=A"    << std::endl
                                    // change base address temporary
                                    << "@" << pos << std::endl
                                    << "M=D+M"  << std::endl
                                    << "@0"     << std::endl
                                    << "AM=M-1"    << std::endl
                                    << "D=M"    << std::endl
                                    << "@" << pos << std::endl
                                    << "A=M"  << std::endl
                                    << "M=D"  << std::endl
                                    << "@" << index << std::endl
                                    << "D=A"  << std::endl
                                    // restore origin value
                                    << "@" << pos << std::endl
                                    << "M=M-D"  << std::endl;
                        address_ += 14;
                    }
                }
                break;
            }
            default:
                break;
        }
    }

    void CodeWriter::WriteLabel(std::string symbol) {
        std::transform(symbol.begin(), symbol.end(), symbol.begin(), toupper);
        (*output_)  << "(" << symbol << ")" << std::endl;
    }

    void CodeWriter::WriteGoto(std::string symbol) {
        std::transform(symbol.begin(), symbol.end(), symbol.begin(), toupper);
        (*output_)  << "@" << symbol << std::endl
                    << "0;JMP" << std::endl;
        address_ += 2;
    }

    void CodeWriter::WriteIf(std::string symbol) {
        std::transform(symbol.begin(), symbol.end(), symbol.begin(), toupper);
        (*output_)  << "@0"     << std::endl
                    << "AM=M-1" << std::endl
                    << "D=M"    << std::endl
                    << "@" << symbol << std::endl
                    << "D;JNE"    << std::endl;
        address_ += 5;
    }

    void CodeWriter::WriteCall(const std::string function_name, const int num_args) {
        // push return-address
        static int identifier = 0;
        identifier++;
        (*output_) << "@" << function_name << "." << identifier << std::endl
                   << "D=A"     << std::endl
                   << "@0"      << std::endl
                   << "A=M"     << std::endl
                   << "M=D"     << std::endl
                   << "@0"      << std::endl
                   << "M=M+1"   << std::endl;
        address_ += 7;

        for (int i = 1; i < 5; ++i) {
            // push LCL ARG THIS THAT 
            (*output_) << "@" << i << std::endl
                       << "D=M"     << std::endl
                       << "@0"      << std::endl
                       << "A=M"     << std::endl
                       << "M=D"     << std::endl
                       << "@0"      << std::endl
                       << "M=M+1"   << std::endl;
            address_ += 7;
        }

        // ARG = SP - n - 5
        (*output_) << "@" << num_args + 5 << std::endl
                   << "D=A"     << std::endl
                   << "@0"      << std::endl
                   << "D=M-D"   << std::endl
                   << "@2"      << std::endl
                   << "M=D"     << std::endl;
        address_ += 6;
        // LCL = SP
        (*output_) << "@0"      << std::endl
                   << "D=M"     << std::endl
                   << "@1"      << std::endl
                   << "M=D"     << std::endl;
        address_ += 4;
        // goto f
        (*output_) << "@" << function_name << std::endl
                   << "0;JMP" << std::endl;
        address_ += 2;
        // (return-address)
        (*output_) << "(" << function_name << "." << identifier << ")" << std::endl;
    }

    void CodeWriter::WriteReturn() {
        // FRAME = LCL
        (*output_)  << "@1"     << std::endl
                    << "D=M"    << std::endl
                    << "@13"    << std::endl
                    << "M=D"   << std::endl
                    // RET = *(FRAME-5)
                    << "@5"     << std::endl
                    << "A=D-A"  << std::endl
                    << "D=M"    << std::endl
                    << "@14"    << std::endl
                    << "M=D"    << std::endl
                    // *ARG = *(SP - 1)
                    << "@0"     << std::endl
                    << "A=M-1"  << std::endl
                    << "D=M"    << std::endl
                    << "@2"     << std::endl
                    << "A=M"    << std::endl
                    << "M=D"    << std::endl
                    // SP = ARG + 1 
                    << "@2"     << std::endl
                    << "D=M+1"  << std::endl
                    << "@0"     << std::endl
                    << "M=D"    << std::endl;
        address_ += 19;

        for (int i = 1; i < 5; ++i) {
            // THAT THIS ARG LCL = *(FRAME - i) 
            (*output_) << "@13"     << std::endl
                       << "D=M"     << std::endl
                       << "@" << i << std::endl
                       << "A=D-A"   << std::endl
                       << "D=M"     << std::endl
                       << "@" << 5 - i << std::endl
                       << "M=D"     << std::endl;
            address_ += 7;
        }
        // goto RET
        (*output_) << "@14"     << std::endl
                   << "A=M"     << std::endl
                   << "0;JMP"   << std::endl;
        address_ += 3;
    }

    void CodeWriter::WriteFunction(const std::string function_name, const int num_locals) {
        (*output_)  << "(" << function_name << ")" << std::endl;
        for (int i = 0; i < num_locals; ++i) {
            // push constant 0
            (*output_) << "@0"      << std::endl
                       << "A=M"     << std::endl
                       << "M=0"     << std::endl
                       << "@0"      << std::endl
                       << "M=M+1"   << std::endl;
            address_ += 5;
        }
    }

    void CodeWriter::Close() {
        if (can_delete_input_) {
            delete reinterpret_cast<std::ofstream*>(output_);
            can_delete_input_ = false;
        }
    }
} // namespace vmtranslator
