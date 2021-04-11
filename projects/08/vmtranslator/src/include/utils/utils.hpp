#include <string>
#include <vector>

namespace vmtranslator
{
    class Utils {
        public:
            /**
             * Retrieve files from directory recursively and store those filename to files
             */
            void static ReadDir(const std::string& dir_or_file, std::vector<std::string>& files);
            /**
             * Store .vm file to files
             */
            void static StoreVMFile(const std::string& file, std::vector<std::string>& files);
    };
} // namespace vmtranslator
