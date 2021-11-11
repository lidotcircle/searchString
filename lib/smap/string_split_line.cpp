#include "smap/string_split_line.h"
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

