#include "smap/string_truncate.h"
#include "smap/mapper_factory.h"
#include <stdexcept>
using namespace std;

TruncateMapper::TruncateMapper(size_t max_length): len(max_length) { }

vector<pair<size_t,string>> TruncateMapper::map(const std::string& str) const
{
    return { make_pair(0, str.substr(0, this->len)) };
}

static auto name = "trun";
static auto desc = "maximum length by truncating string";
static auto creator = [](const string& max_len) {
    size_t pos = 0;
    int len = std::stoi(max_len, &pos);
    if (pos != max_len.size() || len <= 0)
        throw std::runtime_error("please specify a correct positive integer");

    return shared_ptr<StringMapper>(new TruncateMapper(len));
};
const vector<int> TruncateMapper::register_handles = {
    MapperFactory::register_mapper("ascii",  name, desc, creator),
    MapperFactory::register_mapper("gb2312", name, desc, creator),
    MapperFactory::register_mapper("utf8", name, desc, creator),
    MapperFactory::register_mapper("utf16", name, desc, creator),
};
