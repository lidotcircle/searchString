#include "sfilter/exclusive.h"
#include "sfilter/filter_factory.h"
#include "utils.h"
#include <fstream>
#include <stdexcept>
#include <aho_corasick.hpp>
using namespace std;


ExclusiveFilter::ExclusiveFilter(const std::string& file) {
    std::ifstream fx(file, std::ios::binary);

    if (!fx)
        throw std::runtime_error("Could not open file " + file);

    string line;
    while (safe_getline(fx, line))
        this->trie.insert(line);
}

int ExclusiveFilter::filter(const std::string& str) const {
    auto _this = const_cast<ExclusiveFilter*>(this);
    auto result = _this->trie.parse_text(str);
    if (!result.empty())
        return -1;

    return 0;
}

static auto name = "exc";
static auto desc = "file, reject when string contain substring in the file";
static auto creator = [](const string& file) {
    return shared_ptr<StringFilter>(new ExclusiveFilter(file));
};
const vector<int> ExclusiveFilter::register_handles = {
    FilterFactory::register_filter("ascii",  name, desc, creator),
    FilterFactory::register_filter("gb2312", name, desc, creator),
};
