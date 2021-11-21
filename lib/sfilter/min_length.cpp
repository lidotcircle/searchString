#include "sfilter/min_length.h"
#include "sfilter/filter_factory.h"
#include <ctype.h>
#include <stdexcept>
using namespace std;


MiniumLength::MiniumLength(size_t len): min(len) {}

int MiniumLength::filter(const std::string& str) const {
    if (str.size() < this->min) return -1;
    return 0;
}

static auto name = "min";
static auto desc = "minimum length in bytes, positive integer";
static auto creator = [](const string& minlen) {
    if (minlen.empty() || !isdigit(minlen[0]))
        throw std::runtime_error("please specify a correct positive integer");

    size_t pos = 0;
    int len = std::stoi(minlen, &pos);
    if (pos != minlen.size() || len <= 0)
        throw std::runtime_error("please specify a correct positive integer");

    return shared_ptr<StringFilter>(new MiniumLength(len));
};
const vector<int> MiniumLength::register_handles = {
    FilterFactory::register_filter("ascii",  name, desc, creator),
    FilterFactory::register_filter("gb2312", name, desc, creator),
    FilterFactory::register_filter("utf8", name, desc, creator),
    FilterFactory::register_filter("utf16", name, desc, creator),
};
