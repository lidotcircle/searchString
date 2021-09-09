#ifndef _SEARCH_STRING_
#define _SEARCH_STRING_

#include <vector>
#include <type_traits>
#include <iterator>
#include <tuple>
#include <iostream>


/** GB2312 string
 *  1. ascii string (without control characters but include TAB(0x09) LF(0x0A) CR(0x0D))
 *  2. first byte in 0xA1-0xF7 and second byte in 0xA1-0xFE
 */

#define _IS_V_ASCII(c)  ((c >= 0x20 && c <  0x7f) \
        || c == 0x09 || c == 0x0A || c == 0x0D)
#define _IS_V_FIRST(c)  (c >= 0xA1 && c <= 0xF7)
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
    typename InIter,
    typename = typename std::enable_if<
        std::is_convertible<typename std::iterator_traits<InIter>::iterator_category,
            std::input_iterator_tag>::value, void>::type,
    typename = typename std::enable_if<
        std::is_integral<
            typename std::iterator_traits<InIter>::value_type>::value, void>::type,
    typename = typename std::enable_if<
        sizeof(typename std::iterator_traits<InIter>::value_type) == 1, void>::type
    >
std::vector<std::pair<size_t, size_t>> searchGB2312(
    InIter begin, 
    InIter end, 
    size_t min_length, 
    size_t base)
{
    std::vector<std::tuple<GB2312MatchState, size_t, size_t>> candidates;
    std::vector<std::pair<size_t, size_t>> result;

    // char is in range of 0x20-0x7f, no one in astate
    bool newC1 = true;
    // char is in range of 0xa1-0xf7, no one in bstate
    bool newC2 = true;

    for(size_t n=0; begin != end; begin++, n++) {
        unsigned char c = *begin;

        if(!_IS_V_ASCII(c) && !_IS_V_FIRST(c) && !_IS_V_SECOND(c)) {
            clean_candiates(candidates, result, min_length, base);
            candidates.clear();
            continue;
        }

        newC1 = _IS_V_ASCII(c);
        newC2 = _IS_V_FIRST(c);
        decltype(candidates) cc;
        GB2312MatchState s;
        size_t b, e;
        for(auto& candidate: candidates) {
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
            } else if (_IS_V_FIRST(c)) {
                auto ns = s == astate ? bstate : astate;
                cc.push_back(std::make_tuple(ns, b, e + 1));
                if (ns == bstate) {
                    newC2 = false;
                }
            } else {
                if (s == bstate) {
                    cc.push_back(std::make_tuple(astate, b, e + 1));
                } else {
                    result.push_back(std::make_pair(base + b, base + e));
                }
            }
        }
        candidates = std::move(cc);

        if(newC1 || newC2) {
            auto ns = _IS_V_ASCII(c) ? astate : bstate;
            candidates.push_back(std::make_tuple(ns, n, n + 1));
        }
    }

    clean_candiates(candidates, result, min_length, base);
    return result;
}


#endif // _SEARCH_STRING_
