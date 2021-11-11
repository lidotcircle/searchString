#include "sfilter/exclusive.h"
#include <fstream>
#include <stdexcept>
#include <aho_corasick.hpp>
using namespace std;


ExclusiveFilter::ExclusiveFilter(const std::string& file) {
    std::ifstream fx(file, std::ios::binary);

    if (!fx)
        throw std::runtime_error("Could not open file " + file);

    string line;
    while (getline(fx, line))
        this->trie.insert(line);
}

int ExclusiveFilter::filter(const std::string& str) const {
    auto _this = const_cast<ExclusiveFilter*>(this);
    auto result = _this->trie.parse_text(str);
    if (!result.empty())
        return -1;

    return 0;
}

