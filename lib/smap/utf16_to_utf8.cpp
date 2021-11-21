#include "smap/utf16_to_utf8.h"
#include "sfinder/unicode_converter.h"
#include "smap/mapper_factory.h"
using namespace std;


vector<pair<size_t,string>> UTF16ToUTF8Mapper::map(const std::string& str) const
{
    size_t n;
    auto codepoints = decoder_utf16(str, n, false);

    return { make_pair(0, encoder_utf8(codepoints, n)) };
}

static auto name = "utf8";
static auto desc = "convert to utf8 encoding";
static auto creator = [](const string&) {
    return shared_ptr<StringMapper>(new UTF16ToUTF8Mapper());
};
const vector<int> UTF16ToUTF8Mapper::register_handles = {
    MapperFactory::register_mapper("utf16", name, desc, creator),
};
