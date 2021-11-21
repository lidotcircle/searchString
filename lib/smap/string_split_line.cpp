#include "smap/string_split_line.h"
#include "smap/mapper_factory.h"
using namespace std;


vector<pair<size_t,string>> SplitLineMapper::map(const std::string& str) const
{
    vector<pair<size_t,string>> result;
    size_t pos = 0;
    string kx;

    for (size_t i = 0; i < str.size(); ++i)
    {
        auto c = str[i];
        if (c == '\n' || c == '\r')
        {
            if (kx.size() > 0) {
                result.push_back(make_pair(pos, std::move(kx)));
            }
            pos = i + 1;
        }
        else
        {
            kx.push_back(c);
        }
    }

    if (kx.size() > 0)
        result.push_back(make_pair(pos, std::move(kx)));

    return result;
}

static auto name = "splt";
static auto desc = "split line by cr lf";
static auto creator = [](const string&) {
    return shared_ptr<StringMapper>(new SplitLineMapper());
};
const vector<int> SplitLineMapper::register_handles = {
    MapperFactory::register_mapper("ascii",  name, desc, creator),
    MapperFactory::register_mapper("gb2312", name, desc, creator),
    MapperFactory::register_mapper("utf8", name, desc, creator),
    MapperFactory::register_mapper("utf16", name, desc, creator),
};
