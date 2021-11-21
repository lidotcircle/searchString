#include "sfilter/exclusive_full.h"
#include "sfilter/filter_factory.h"
#include "utils.h"
#include <fstream>
#include <stdexcept>
#include <aho_corasick.hpp>
using namespace std;


ExclusiveFullFilter::ExclusiveFullFilter(const std::string& file) {
    std::ifstream fx(file, std::ios::binary);

    if (!fx)
        throw std::runtime_error("Could not open file " + file);

    string line;
    while (safe_getline(fx, line))
        this->exc_list.insert(line);
}

int ExclusiveFullFilter::filter(const std::string& str) const {
    if (this->exc_list.find(str) != this->exc_list.end())
        return -1;
    
    return 0;
}

static auto name = "exf";
static auto desc = "file, reject when string contain string in the file";
static auto creator = [](const string& file) {
    return shared_ptr<StringFilter>(new ExclusiveFullFilter(file));
};
const vector<int> ExclusiveFullFilter::register_handles = {
    FilterFactory::register_filter("ascii",  name, desc, creator),
    FilterFactory::register_filter("gb2312", name, desc, creator),
    FilterFactory::register_filter("utf8", name, desc, creator),
    FilterFactory::register_filter("utf16", name, desc, creator),
};
