#include <vector>

#include "code_writer.hpp"

namespace vmtranslator {
    CodeWriter::CodeWriter(std::ostream* output): output_(output), can_delete_input_(false), address_(0) {
    }
    CodeWriter::CodeWriter(const std::string file): output_(nullptr), can_delete_input_(true), address_(0) {
        SetFileName(file);
    }

    CodeWriter::~CodeWriter() {
        Close();
    };

    void CodeWriter::SetFileName(const std::string file_name) {
        std::ofstream* open_file_p = new std::ofstream(file_name);
        if (!open_file_p->is_open()) {
            delete open_file_p;
            throw std::exception();
        }
        can_delete_input_ = true;
        if (output_ != nullptr) {
            (*open_file_p) << output_->rdbuf();
        }
        output_ = open_file_p;

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

    void CodeWriter::Close() {
        if (can_delete_input_) {
            delete reinterpret_cast<std::ofstream*>(output_);
            can_delete_input_ = false;
        }
    }
} // namespace vmtranslator
