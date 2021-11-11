#include "sfilter/exclusive_full.h"
#include <fstream>
#include <stdexcept>
#include <aho_corasick.hpp>
using namespace std;


ExclusiveFullFilter::ExclusiveFullFilter(const std::string& file) {
    std::ifstream fx(file, std::ios::binary);

    if (!fx)
        throw std::runtime_error("Could not open file " + file);

    string line;
    while (getline(fx, line))
        this->exc_list.insert(line);
}

int ExclusiveFullFilter::filter(const std::string& str) const {
    if (this->exc_list.find(str) != this->exc_list.end())
        return -1;

    return 0;
}

