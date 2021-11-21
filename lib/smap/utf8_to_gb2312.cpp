#include "smap/utf8_to_gb2312.h"
#include "sfinder/unicode_converter.h"
#include "smap/mapper_factory.h"
#include "utils.h"
using namespace std;


vector<pair<size_t,string>> UTF8ToGB2312Mapper::map(const std::string& str) const
{
    size_t n;
    auto codepoints = decoder_utf8(str, n);
    auto vx = unicode_to_gb2312(codepoints);
    vector<uint16_t> vals(vx.begin(), vx.end());

    return { make_pair(0, twobytes2gb2312str(vals)) };
}

static auto name = "gb2312";
static auto desc = "convert to gb2312 encoding";
static auto creator = [](const string&) {
    return shared_ptr<StringMapper>(new UTF8ToGB2312Mapper());
};
const vector<int> UTF8ToGB2312Mapper::register_handles = {
    MapperFactory::register_mapper("utf8", name, desc, creator),
};
