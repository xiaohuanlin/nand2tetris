#include <bitset>
#include <string>
#include <unordered_map>

namespace compiler {
    class Code {
        public:
            Code() = default;
            ~Code() = default;
            /**
             * Judge if the comp code is right
             */
            bool IsValidCompCode(const std::string& code);
            /**
             * Judge if the dest code is right
             */
            bool IsValidDestCode(const std::string& code);
            /**
             * Judge if the jump code is right
             */
            bool IsValidJumpCode(const std::string& code);
            /**
             * The bits representation of dest
             */
            inline std::bitset<3> Dest(std::string dest) {return (*dest_mapping_.find(dest)).second;};
            /**
             * The bits representation of comp
             */
            inline std::bitset<7> Comp(std::string comp) {return (*comp_mapping_.find(comp)).second;};
            /**
             * The bits representation of jump
             */
            inline std::bitset<3> Jump(std::string jump) {return (*jump_mapping_.find(jump)).second;};
        private:
            std::unordered_map<std::string, std::bitset<3>> dest_mapping_ {
                {"null",    std::bitset<3>("000")},
                {"M",       std::bitset<3>("001")},
                {"D",       std::bitset<3>("010")},
                {"MD",      std::bitset<3>("011")},
                {"A",       std::bitset<3>("100")},
                {"AM",      std::bitset<3>("101")},
                {"AD",      std::bitset<3>("110")},
                {"AMD",     std::bitset<3>("111")},
            }; 
            std::unordered_map<std::string, std::bitset<3>> jump_mapping_ {
                {"null",    std::bitset<3>("000")},
                {"JGT",     std::bitset<3>("001")},
                {"JEQ",     std::bitset<3>("010")},
                {"JGE",     std::bitset<3>("011")},
                {"JLT",     std::bitset<3>("100")},
                {"JNE",     std::bitset<3>("101")},
                {"JLE",     std::bitset<3>("110")},
                {"JMP",     std::bitset<3>("111")},
            }; 
            std::unordered_map<std::string, std::bitset<7>> comp_mapping_ {
                {"0",       std::bitset<7>("0101010")},
                {"1",       std::bitset<7>("0111111")},
                {"-1",      std::bitset<7>("0111010")},
                {"D",       std::bitset<7>("0001100")},
                {"A",       std::bitset<7>("0110000")},
                {"!D",      std::bitset<7>("0001101")},
                {"!A",      std::bitset<7>("0110001")},
                {"-D",      std::bitset<7>("0001111")},
                {"-A",      std::bitset<7>("0110011")},
                {"D+1",     std::bitset<7>("0011111")},
                {"A+1",     std::bitset<7>("0110111")},
                {"D-1",     std::bitset<7>("0001110")},
                {"A-1",     std::bitset<7>("0110010")},
                {"D+A",     std::bitset<7>("0000010")},
                {"D-A",     std::bitset<7>("0010010")},
                {"A-D",     std::bitset<7>("0000111")},
                {"D&A",     std::bitset<7>("0000000")},
                {"D|A",     std::bitset<7>("0010101")},
                {"M",       std::bitset<7>("1110000")},
                {"!M",      std::bitset<7>("1110001")},
                {"-M",      std::bitset<7>("1110011")},
                {"M+1",     std::bitset<7>("1110111")},
                {"M-1",     std::bitset<7>("1110010")},
                {"D+M",     std::bitset<7>("1000010")},
                {"D-M",     std::bitset<7>("1010011")},
                {"M-D",     std::bitset<7>("1000111")},
                {"D&M",     std::bitset<7>("1000000")},
                {"D|M",     std::bitset<7>("1010101")},
            }; 
    };
} // namespace compiler
