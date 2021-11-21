#include "sfilter/inclusive.h"
#include "sfilter/filter_factory.h"
#include "utils.h"
#include <fstream>
#include <stdexcept>
#include <aho_corasick.hpp>
using namespace std;


InclusiveFilter::InclusiveFilter(const std::string& file, bool strict): strict(strict) {
    std::ifstream fx(file, std::ios::binary);

    if (!fx)
        throw std::runtime_error("Could not open file " + file);

    string line;
    while (safe_getline(fx, line))
        this->trie.insert(line);
}

int InclusiveFilter::filter(const std::string& str) const {
    auto _this = const_cast<InclusiveFilter*>(this);
    auto result = _this->trie.parse_text(str);
    if (!result.empty())
        return 1;

    return strict ? -1 : 0;
}

static auto name = "inc";
static auto desc = "file[:strict] accept when string contain at least one substring in the file";
static auto creator = [](const string& param) {
    bool strict = false;
    auto file = param;
    auto pos = param.find(':');
    if (pos != std::string::npos) {
        auto s = param.substr(pos + 1);
        if (s == "strict")
            strict = true;
        else
            throw std::runtime_error("please specify a correct parameter, file[:strict]");
        file = param.substr(0, pos);
    }

    return shared_ptr<StringFilter>(new InclusiveFilter(file, strict));
};
const vector<int> InclusiveFilter::register_handles = {
    FilterFactory::register_filter("ascii",  name, desc, creator),
    FilterFactory::register_filter("gb2312", name, desc, creator),
    FilterFactory::register_filter("utf8", name, desc, creator),
    FilterFactory::register_filter("utf16", name, desc, creator),
};
