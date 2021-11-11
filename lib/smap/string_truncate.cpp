#include "smap/string_truncate.h"
using namespace std;

TruncateMapper::TruncateMapper(size_t max_length): len(max_length) { }

vector<pair<size_t,string>> TruncateMapper::map(const std::string& str) const
{
    return { make_pair(0, str.substr(0, this->len)) };
}

