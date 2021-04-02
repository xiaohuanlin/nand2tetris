#include <string>
#include <bitset>

namespace compiler {
    class Code {
        public:
            Code();
            ~Code();
            /**
             * The bits representation of dest
             */
            std::bitset<3> Dest(std::string dest);
            /**
             * The bits representation of comp
             */
            std::bitset<7> Comp(std::string comp);
            /**
             * The bits representation of jump
             */
            std::bitset<3> Jump(std::string jump);
        private:
    };
} // namespace compiler
