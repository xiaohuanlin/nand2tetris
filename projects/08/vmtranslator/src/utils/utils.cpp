#include <dirent.h>
#include <iostream>
#include <sys/stat.h>

#include "utils/utils.hpp"

namespace vmtranslator 
{
    void Utils::ReadDir(const std::string& dir, std::vector<std::string>& files) {
        struct dirent* ent;
        DIR* dir_p;

        if ((dir_p = opendir(dir.c_str())) == nullptr) {
            StoreVMFile(dir, files);
            return;
        }

        while ((ent = readdir(dir_p)) != nullptr) {
            std::string dir_name(ent->d_name);
            if (dir_name == ".." || dir_name == ".") {
                // skip the current node and parent node
                continue;
            }
            StoreVMFile(dir + "/" + dir_name, files);
        }

        closedir(dir_p);
    };

    void Utils::StoreVMFile(const std::string& file, std::vector<std::string>& files) {
        struct stat buf;

        if (stat(file.c_str(), &buf) != 0) {
            perror(file.c_str());
            return;
        }

        if (S_ISDIR(buf.st_mode)) {
            ReadDir(file, files);
        } else if (S_ISREG(buf.st_mode)) {
            // only load .vm file
            size_t last_dot = file.find_last_of('.');
            if (last_dot == std::string::npos) {
                return;
            }
            if (std::string(file, last_dot + 1) != "vm") {
                return;
            }

            std::cout << "read file: " << file << std::endl;
            files.push_back(std::move(file));
        }
    }

} // namespace vmtranslator 
