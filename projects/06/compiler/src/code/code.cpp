#include "code.hpp"

namespace compiler
{
    bool Code::IsValidCompCode(const std::string& code) {
        return comp_mapping_.find(code) != comp_mapping_.end();
    }

    bool Code::IsValidDestCode(const std::string& code) {
        return dest_mapping_.find(code) != dest_mapping_.end();
    }

    bool Code::IsValidJumpCode(const std::string& code) {
        return jump_mapping_.find(code) != jump_mapping_.end();
    }
    
} // namespace compiler
