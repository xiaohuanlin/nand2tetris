#include <dirent.h>
#include <cstring>
#include <sys/stat.h>
#include <vector>

#include "code_writer.hpp"

void ReadDir(std::string dir, std::vector<std::string>& files);
void StoreFile(std::string file, std::vector<std::string>& files) {
    struct stat buf;

    if (stat(file.c_str(), &buf) != 0) {
        perror(file.c_str());
        return;
    }

    if (S_ISDIR(buf.st_mode)) {
        ReadDir(file, files);
    } else if (S_ISREG(buf.st_mode)) {
        // only load .vm file
        std::string file_name(file);
        size_t last_dot = file_name.find_last_of('.');
        if (last_dot == std::string::npos) {
            return;
        }
        if (std::string(file_name, last_dot + 1) != "vm") {
            return;
        }

        std::cout << "read file: " << file_name << std::endl;
        files.push_back(file_name);
    }
}

void ReadDir(std::string dir, std::vector<std::string>& files) {
    struct dirent* ent;
    DIR* dir_p;

    if ((dir_p = opendir(dir.c_str())) == nullptr) {
        StoreFile(dir, files);
        return;
    }

    while ((ent = readdir(dir_p)) != nullptr) {
        if (strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".") == 0) {
            continue;
        }
        StoreFile(dir + "/" + std::string(ent->d_name), files);
    }

    closedir(dir_p);
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 1;
    }

    char* input_file_name_or_dir = argv[1];
    std::vector<std::string> filenames;
    ReadDir(input_file_name_or_dir, filenames);

    if (filenames.empty()) {
        std::cout << "empty file or dir" << std::endl;
        return 1;
    }

    // set output file name
    std::string base_out_file_name = filenames[0];
    if (filenames.size() > 1) {
        // more than one file, we create the output file into current directory
        base_out_file_name = "./main.asm";
    }
    std::size_t pos = base_out_file_name.length();
    if ((pos = base_out_file_name.find_last_of('.')) != std::string::npos) {
        base_out_file_name.erase(pos);
    }
    std::string out_file_name = base_out_file_name + ".asm";
    if (argc > 2) {
        out_file_name = argv[2];
    }
    std::cout << "write file: " << out_file_name << std::endl;
    vmtranslator::CodeWriter code_writer(out_file_name);

    code_writer.WriteInit();

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
                    break;
            }
        }
    }
    code_writer.Close();
}