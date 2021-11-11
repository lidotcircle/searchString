#include "sfilter/inclusive.h"
#include <fstream>
#include <stdexcept>
#include <aho_corasick.hpp>
using namespace std;


InclusiveFilter::InclusiveFilter(const std::string& file, bool strict): strict(strict) {
    std::ifstream fx(file, std::ios::binary);

    if (!fx)
        throw std::runtime_error("Could not open file " + file);

    string line;
    while (getline(fx, line))
        this->trie.insert(line);
}

int InclusiveFilter::filter(const std::string& str) const {
    auto _this = const_cast<InclusiveFilter*>(this);
    auto result = _this->trie.parse_text(str);
    if (!result.empty())
        return 1;

    return strict ? -1 : 0;
}
