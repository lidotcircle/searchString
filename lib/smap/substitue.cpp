#include "smap/substitute.h"
#include "smap/mapper_factory.h"
#include "sfinder/unicode_converter.h"
#include <regex>
using namespace std;

SubstituteMapper::SubstituteMapper(const string& encoding, const string& pattern, const string& replacement)
    : pattern(pattern, std::regex::ECMAScript), replacement(replacement), encoding(encoding)
{
}

std::vector<std::pair<size_t,std::string>> SubstituteMapper::map(const std::string& str) const {
    string utf8str;

    if (this->encoding == "utf8" || this->encoding == "ascii") {
        utf8str = str;
    } else if (this->encoding == "utf16") {
        size_t n;
        utf8str = encoder_utf8(decoder_utf16(str, n), n);
    } else if (this->encoding == "gb2312") {
        size_t n;
        utf8str = encoder_utf8(gb2312str_to_unicode(str), n);
    } else {
        throw runtime_error("Unsupported encoding: " + this->encoding);
    }

    auto rstr = regex_replace(utf8str, pattern, replacement);

    if (this->encoding == "utf16") {
        size_t n;
        rstr = encoder_utf16(decoder_utf8(rstr, n), n);
    } else if (this->encoding == "gb2312") {
        size_t n;
        rstr = unicode_to_gb2312str(decoder_utf8(str, n));
    }

    return { { 0, rstr } };
}

static auto name = "sub";
static auto desc = "substitute with regex, regex/format";
template<const char* encoding>
static auto sub_creator = [](const string& substitute_str) {
    size_t p1 = substitute_str.find('/');
    if (p1 == 0)
        throw runtime_error("Invalid substitute string, require '/': " + substitute_str);
    while (p1 != string::npos && substitute_str[p1 - 1] == '\\') {
        p1 = substitute_str.find('/', p1 + 1);
    }
    if (p1 == string::npos || p1 == substitute_str.size() - 1)
        throw runtime_error("Invalid substitute string, not empty regex and replacement: " + substitute_str);

    auto pattern = substitute_str.substr(0, p1);
    auto replacement = substitute_str.substr(p1 + 1);

    return shared_ptr<StringMapper>(new SubstituteMapper(encoding, pattern, replacement));
};
static const char encoding_gb2312[] = "gb2312";
static const char encoding_ascii[]  = "ascii";
static const char encoding_utf8[]   = "utf8";
static const char encoding_utf16[]  = "utf16";
const vector<int> SubstituteMapper::register_handles = {
    MapperFactory::register_mapper("ascii",  name, desc, sub_creator<encoding_gb2312>),
    MapperFactory::register_mapper("gb2312", name, desc, sub_creator<encoding_ascii>),
    MapperFactory::register_mapper("utf8",   name, desc, sub_creator<encoding_utf8>),
    MapperFactory::register_mapper("utf16",  name, desc, sub_creator<encoding_utf16>),
};
