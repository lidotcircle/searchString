#include "utils.h"
#include <filesystem>

namespace fs = std::filesystem;


std::vector<std::string> ls_files_recursively(const std::string& dir) {
    std::vector<std::string> ans;

    for(auto& p: fs::recursive_directory_iterator(dir)) {
        if(p.is_regular_file())
            ans.push_back(p.path().string());
    }

    return ans;
}

