#include "main/main.hpp"

int main(int argc, char* argv[]) {
    vmtranslator::Main m(true);
    return m.Run(argc, argv);
}