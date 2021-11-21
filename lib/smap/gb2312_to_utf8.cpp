#include "smap/gb2312_to_utf8.h"
#include "sfinder/unicode_converter.h"
#include "smap/mapper_factory.h"
#include "utils.h"
using namespace std;


vector<pair<size_t,string>> GB2312ToUTF8Mapper::map(const std::string& str) const
{
    size_t n;
    auto gb2312codes = gb2312str2twobytes(str);
    vector<int> vals(gb2312codes.begin(), gb2312codes.end());
    auto codepoints = gb2312_to_unicode(vals);

    return { make_pair(0, encoder_utf8(codepoints, n)) };
}

static auto name = "utf8";
static auto desc = "convert to utf8 encoding";
static auto creator = [](const string&) {
    return shared_ptr<StringMapper>(new GB2312ToUTF8Mapper());
};
const vector<int> GB2312ToUTF8Mapper::register_handles = {
    MapperFactory::register_mapper("gb2312", name, desc, creator),
};
