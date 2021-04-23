#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "writer/node.hpp"

namespace jack_compiler {
    /**
     * The base class for all writer
     */
    class Writer {
    public:
        explicit Writer(const std::string& file);
        explicit Writer(std::ostream* output);
        virtual ~Writer();
        /**
         * Write the whole content of root and its relatived nodes
         */
        virtual void WriteToOutput(std::shared_ptr<Node> root);
    protected:
        virtual std::string Write(std::shared_ptr<Node> root, int level = 0)=0;
        bool can_delete_output_;
        std::ostream* output_;
    };
} // namespace jack_compiler