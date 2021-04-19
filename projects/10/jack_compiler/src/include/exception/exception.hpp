#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include <memory>

namespace jack_compiler {
    #define THROW_COMPILER_EXCEPT {throw CompileException(std::string(__FILE__) + ":" + std::to_string(__LINE__));}

    class CompileException : public std::logic_error {
    public:
        CompileException(const std::string& info): std::logic_error(info), info_(info) {};

        const char* what() const noexcept {
            // todo memort leak
            const char* res = (new std::string("Compiler error: " + info_))->c_str();
            return res;
        };
    protected:
        std::string info_;
    };
}