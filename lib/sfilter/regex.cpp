#include "sfilter/regex.h"
#include "sfilter/filter_factory.h"
#include "sfinder/unicode_converter.h"
#include <regex>
#include <stdexcept>
#include <iostream>
using namespace std;


RegexFilter::RegexFilter(const string& encoding, const string& pattern):
    m_encoding(encoding), m_regex(pattern, std::regex::ECMAScript)
{
}

int RegexFilter::filter(const string& str) const {
    string utf8str;

    if (this->m_encoding == "utf8" || this->m_encoding == "ascii") {
        utf8str = str;
    } else if (this->m_encoding == "utf16") {
        size_t n;
        utf8str = encoder_utf8(decoder_utf16(str, n), n);
    } else if (this->m_encoding == "gb2312") {
        size_t n;
        utf8str = encoder_utf8(gb2312str_to_unicode(str), n);
    } else {
        throw runtime_error("Unsupported encoding: " + this->m_encoding);
    }

    return std::regex_match(utf8str, this->m_regex) ? 0 : -1;
}

static auto name = "regex";
static auto desc = "regular expression with ECMAScript syntax, <:regex>";
template<const char* encoding>
static auto regex_creator = [](const string& regex) {
    return std::shared_ptr<StringFilter>(new RegexFilter(encoding, regex));
};

static const char encoding_gb2312[] = "gb2312";
static const char encoding_ascii[]  = "ascii";
static const char encoding_utf8[]   = "utf8";
static const char encoding_utf16[]  = "utf16";
const vector<int> RegexFilter::register_handles = {
    FilterFactory::register_filter("gb2312", name, desc, regex_creator<encoding_gb2312>),
    FilterFactory::register_filter("ascii",  name, desc, regex_creator<encoding_ascii>),
    FilterFactory::register_filter("utf8",   name, desc, regex_creator<encoding_utf8>),
    FilterFactory::register_filter("utf16",  name, desc, regex_creator<encoding_utf16>),
};
