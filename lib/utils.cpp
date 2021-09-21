#include "utils.h"
#include <filesystem>
#include <assert.h>
#include <exception>
#include <map>

namespace fs = std::filesystem;


std::vector<std::string> ls_files_recursively(const std::string& dir) {
    std::vector<std::string> ans;

    for(auto& p: fs::recursive_directory_iterator(dir)) {
        if(p.is_regular_file())
            ans.push_back(p.path().string());
    }

    return ans;
}

static const char HexMapping[] = "0123456789ABCDEF";
std::string str2hexstr(const std::string& str) {
    std::string ans;
    for(auto c: str) {
        unsigned char n1 = c & 0xF0;
        n1 >>= 4;
        unsigned char n2 = c & 0x0F;
        ans.push_back(HexMapping[n1]);
        ans.push_back(HexMapping[n2]);
    }

    return ans;
}

static const std::map<char,char> HexReverseMapping = {
    {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
    {'8', 8}, {'9', 9}, 
    {'A', 10}, {'B', 11}, {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15},
    {'a', 10}, {'b', 11}, {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15},
};
std::string hexstr2str(const std::string& hexstr) {
    assert(hexstr.size() % 2 == 0);
    std::string ans;

    for(int i=0;i<hexstr.size();i+=2) {
        char c1 = hexstr[i];
        char c2 = hexstr[i+1];
        if (HexReverseMapping.find(c1) == HexReverseMapping.end()
         || HexReverseMapping.find(c2) == HexReverseMapping.end())
        {
            throw std::runtime_error("unexpect char " + c1 + c2);
        }
        char c = HexReverseMapping.find(c1)->second;
        c <<= 4;
        c += HexReverseMapping.find(c2)->second;
        ans.push_back(c);
    }

    return ans;
}
