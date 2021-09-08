#ifndef _SEARCH_STRING_
#define _SEARCH_STRING_

#include <vector>
#include <type_traits>
#include <iterator>
#include <tuple>


/** GB2312 string
 *  1. ascii string (without control characters)
 *  2. first byte in 0xA1-0xF7 and second byte in 0xA1-0xFE
 */

#define _IS_V_ASCII (c) (c >= 0x20 && c <  0x7f)
#define _IS_V_FIRST (c) (c >= 0xA1 && c <= 0xF7)
#define _IS_V_SECOND(c) (c >= 0x20 && c <= 0xFE)

enum GB2312MatchState {
    astate,    // complete gb2312 string
    bstate,    // still require additional byte (0xA1 - 0xFE)
};

void clean_candiates(std::vector<std::tuple<GB2312MatchState, size_t, size_t>>& candiates,
                     std::vector<std::pair<size_t, size_t>>& result,
                     size_t min_len, size_t base)
{
    GB2312MatchState s;
    size_t b, e;
    for(auto& candidate: candiates) {
        std::tie(s, b, e) = candidate;

        if(s == bstate) {
            e--;
        }

        if(e - b >= min_len) {
            result.push_back(std::make_pair(b + base, e + base));
        }
    }
}

template<
    typename ForwIter, 
    typename = std::enable_if_t<
        std::is_same_v<typename std::iterator_traits<ForwIter>::iterator_category,
            std::forward_iterator_tag>, void>,
    typename = std::enable_if_t<
        std::is_integral_v<typename std::iterator_traits<ForwIter>::value_type>, void>,
    typename = std::enable_if_t<
        sizeof(typename std::iterator_traits<ForwIter>::value_type) == 8, void>
    >
std::vector<std::pair<size_t, size_t>> searchGB2312(
    ForwIter begin, 
    ForwIter end, 
    size_t min_length, 
    size_t base)
{
    std::vector<std::tuple<GB2312MatchState, size_t, size_t>> candidates;
    std::vector<std::pair<size_t, size_t>> result;

    // char is in range of 0x20-0x7f, no one in astate
    bool newC1 = true;
    // char is in range of 0xa1-0xf7, no one in bstate
    bool newC2 = true;

    for(size_t n=0;begin != end; begin++, n++) {
        unsigned char c = *begin;

        if(!_IS_V_ASCII(c) && !_IS_V_FIRST(c) && !_IS_V_SECOND(c)) {
            clean_candiates(candidates, result, min_length);
            continue;
        }

        newC1 = _IS_V_ASCII(c);
        newC2 = _IS_V_FIRST(c);
        typename decltype(candidates) cc;
        for(auto& candiate: candiates) {
            std::tie(s, b, e) = candidate;

            if(_IS_V_ASCII(c)) {
                if (s == astate) {
                    cc.push_back(std::make_tuple(astate, b, e + 1));
                    newC1 = false;
                } else {
                    if (e - b > min_length + 1) {
                        result.push_back(std::make_pair(base + b, base + e - 1));
                    }
                }
            } else if (_IS_V_FIRST) {
                auto ns = s == astate ? bstate : astate;
                cc.push_back(std::make_tuple(ns, b, e + 1));
                if (ns == bstate) {
                    newC2 = false;
                }
            }
        }
        candidates = std::move(cc);

        if(newC1 || newC2) {
            auto ns = _IS_V_ASCII(c) ? astate : bstate;
            candidates.push_back(std::make_tuple(ns, n, n + 1));
        }
    }

    clean_candiates(candidates, result, min_length);
    return result;
}


#endif // _SEARCH_STRING_
