#pragma once

namespace vmtranslator {
    class Main {
        public:
            Main(bool write_init);
            /**
             * The main function for whole project
             */
            int Run(int argc, char* argv[]);
        private:
            bool write_init_;
    };
} // namespace vmtranslator