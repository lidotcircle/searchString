#include "smap/utf8_to_utf16.h"
#include "sfinder/unicode_converter.h"
#include "smap/mapper_factory.h"
using namespace std;


vector<pair<size_t,string>> UTF8ToUTF16Mapper::map(const std::string& str) const
{
    size_t n;
    auto codepoints = decoder_utf8(str, n);

    return { make_pair(0, encoder_utf16(codepoints, n, false)) };
}

static auto name = "utf16";
static auto desc = "convert to utf16 encoding";
static auto creator = [](const string&) {
    return shared_ptr<StringMapper>(new UTF8ToUTF16Mapper());
};
const vector<int> UTF8ToUTF16Mapper::register_handles = {
    MapperFactory::register_mapper("utf8", name, desc, creator),
};
