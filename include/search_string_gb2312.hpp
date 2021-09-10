#ifndef _SEARCH_STRING_GB2312_
#define _SEARCH_STRING_GB2312_

#include <vector>
#include <type_traits>
#include <iterator>
#include <tuple>
#include <string>
#include <queue>
#include <iostream>
#include "search_string.hpp"


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

template<typename IterT>
class SearchStringGB2312InputIter: public SearchStringInputIter<IterT> {
    using output_array_t = typename SearchStringInputIter<IterT>::output_array_t;
    using candidate_array_t = typename SearchStringInputIter<IterT>::candidate_array_t;

    void clean_candiates(candidate_array_t& candidates, output_array_t& output)
    {
        GB2312MatchState s;
        size_t b, e;
        std::string str;
        for(auto& candidate: candidates) {
            std::tie(s, b, e, str) = candidate;

            if(s == bstate) {
                e--;

                if(this->generate_string()) {
                    str.pop_back();
                }
            }

            if(e - b >= this->min_string_length()) {
                output.push_back(std::make_tuple(b, e, std::move(str)));
            }
        }
    }

    virtual output_array_t feed_char(
            size_t pos, 
            unsigned char c, 
            candidate_array_t& candidates) override
    {
        output_array_t    ans;
        candidate_array_t new_candidates;

        if(!_IS_V_ASCII(c) && !_IS_V_FIRST(c) && !_IS_V_SECOND(c)) {
            clean_candiates(candidates, ans);
            candidates.clear();
            return ans;
        }

        bool newC1 = _IS_V_ASCII(c);
        bool newC2 = _IS_V_FIRST(c);
        decltype(candidates) cc;
        GB2312MatchState s;
        size_t b, e;
        std::string str;
        for(auto& candidate: candidates) {
            std::tie(s, b, e, str) = candidate;

            if(_IS_V_ASCII(c)) {
                if (s == astate) {
                    if(this->generate_string())
                        str.push_back(c);
                    cc.push_back(std::make_tuple(astate, b, e + 1, str));
                    newC1 = false;
                } else {
                    if (e - b >= this->min_string_length() + 1) {
                        if (this->generate_string())
                            str.pop_back();
                        ans.push_back(std::make_tuple(b, e - 1, str));
                    }
                }
            } else if (_IS_V_FIRST(c)) {
                auto ns = s == astate ? bstate : astate;
                if(this->generate_string())
                    str.push_back(c);
                cc.push_back(std::make_tuple(ns, b, e + 1, str));
                if (ns == bstate) {
                    newC2 = false;
                }
            } else {
                if (s == bstate) {
                    if(this->generate_string())
                        str.push_back(c);
                    cc.push_back(std::make_tuple(astate, b, e + 1, str));
                } else if (e - b >= this->min_string_length()) {
                    ans.push_back(std::make_tuple(b, e, str));
                }
            }
        }
        candidates = std::move(cc);

        if(newC1 || newC2) {
            auto ns = _IS_V_ASCII(c) ? astate : bstate;
            std::string str;
            if(this->generate_string())
                str.push_back(c);
            candidates.push_back(std::make_tuple(ns, pos, pos + 1, str));
        }

        candidates = new_candidates;
        return ans;
    }

    public:
    SearchStringGB2312InputIter(IterT b, IterT e, bool g, size_t m):
        SearchStringInputIter<IterT>(b, e, g, m) {}

    SearchStringGB2312InputIter() = default;
};

template<typename T>
SearchStringGB2312InputIter<T> gb2312Begin(T inputIterBegin, T inputIterEnd, bool generate_string, size_t min_len) {
    return SearchStringGB2312InputIter<T>(inputIterBegin, inputIterEnd, generate_string, min_len);
}
template<typename T>
SearchStringGB2312InputIter<T> gb2312End(T inputIter, T inputIterEnd) {return SearchStringGB2312InputIter<T>();}

#endif // _SEARCH_STRING_GB2312_

