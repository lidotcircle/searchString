#include "smap/gb2312_longest_valid_substr.h"
#include "utils.h"
#include "sfilter/gb2312_frequency.h"
using namespace std;

vector<pair<size_t,string>> GB2312LongestValidSubstr::map(const std::string& str) const
{
    auto twobytes = gb2312str2twobytes(str);
    auto npos = std::string::npos;
    size_t max_len = 0;
    size_t max_start = npos;
    size_t cur_start = npos;

    for (size_t i = 0; i < twobytes.size(); ++i)
    {
        if (gb2312_bitset.test(twobytes[i])) {
            if (cur_start == npos)
                cur_start = i;
        } else {
            if (cur_start == npos)
                continue;

            if (i - cur_start > max_len) {
                max_len = i - cur_start;
                max_start = cur_start;
            }

            cur_start = npos;
        }
    }

    if (cur_start != npos) {
        if (twobytes.size() - cur_start > max_len) {
            max_len = twobytes.size() - cur_start;
            max_start = cur_start;
        }
    }

    if (max_start == npos)
        return vector<pair<size_t,string>>();

    vector<uint16_t> bex(twobytes.begin(), twobytes.begin() + max_start);
    vector<uint16_t> eex(twobytes.begin() + max_start, twobytes.begin() + max_start + max_len);

    return {make_pair(bex.size(), twobytes2gb2312str(eex))};
}

