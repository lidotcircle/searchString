#ifndef _SEARCH_STRING_GB2312_
#define _SEARCH_STRING_GB2312_

#include <vector>
#include <type_traits>
#include <iterator>
#include <tuple>
#include <string>
#include <queue>
#include "search_string.hpp"


/** GB2312 string
 *  1. ascii string (without control characters but include TAB(0x09) LF(0x0A) CR(0x0D))
 *  2. first byte in 0xA1-0xF7 and second byte in 0xA1-0xFE
 */

#define _IS_V_ASCII_CRLF(c)  ((c >= 0x20 && c <  0x7f) \
        || c == 0x09 || c == 0x0A || c == 0x0D)
#define _IS_V_ASCII_NOCRLF(c)  ((c >= 0x20 && c <  0x7f) || c == 0x09)
#define _IS_V_FIRST(c)  (c >= 0xA1 && c <= 0xF7)
#define _IS_V_SECOND(c) (c >= 0x20 && c <= 0xFE)

enum GB2312MatchState {
    astate = 1,    // complete gb2312 string
    bstate,        // still require additional byte (0xA1 - 0xFE)
};

template<typename IterT>
class SearchStringGB2312InputIter: public SearchStringInputIter<IterT> {
    using output_array_t = typename SearchStringInputIter<IterT>::output_array_t;
    using candidate_array_t = typename SearchStringInputIter<IterT>::candidate_array_t;
    bool contain_crlf;

    bool is_ascii(unsigned char c) {
        return contain_crlf ? _IS_V_ASCII_CRLF(c) : _IS_V_ASCII_NOCRLF(c);
    }

    void clean_candiates(candidate_array_t& candidates, output_array_t& output)
    {
        GB2312MatchState s;
        size_t b, e;
        std::string str;
        for(auto& candidate: candidates) {
            std::tie((int&)s, b, e, str) = candidate;

            if(s == bstate) {
                e--;

                if(this->generate_string()) {
                    str.pop_back();
                }
            }

            output.push_back(std::make_tuple(b, e, std::move(str)));
        }
    }

    virtual output_array_t feed_char(
            size_t pos, 
            unsigned char c, 
            candidate_array_t& candidates) override
    {
        output_array_t    ans;
        candidate_array_t new_candidates;

        if(!is_ascii(c) && !_IS_V_FIRST(c) && !_IS_V_SECOND(c)) {
            clean_candiates(candidates, ans);
            candidates.clear();
            return ans;
        }

        bool newC1 = is_ascii(c);
        bool newC2 = _IS_V_FIRST(c);
        GB2312MatchState s;
        size_t b, e;
        std::string str;
        for(auto& candidate: candidates) {
            std::tie((int&)s, b, e, str) = candidate;

            if(is_ascii(c)) {
                if (s == astate) {
                    if(this->generate_string())
                        str.push_back(c);
                    new_candidates.push_back(std::make_tuple(astate, b, e + 1, str));
                    newC1 = false;
                } else {
                    if (this->generate_string())
                        str.pop_back();
                    ans.push_back(std::make_tuple(b, e - 1, str));
                }
            } else if (_IS_V_FIRST(c)) {
                auto ns = s == astate ? bstate : astate;
                if(this->generate_string())
                    str.push_back(c);
                new_candidates.push_back(std::make_tuple(ns, b, e + 1, str));
                if (ns == bstate) {
                    newC2 = false;
                }
            } else {
                if (s == bstate) {
                    if(this->generate_string())
                        str.push_back(c);
                    new_candidates.push_back(std::make_tuple(astate, b, e + 1, str));
                } else {
                    ans.push_back(std::make_tuple(b, e, str));
                }
            }
        }
        candidates = std::move(new_candidates);

        if(newC1 || newC2) {
            auto ns = is_ascii(c) ? astate : bstate;
            std::string str;
            if(this->generate_string())
                str.push_back(c);
            candidates.push_back(std::make_tuple(ns, pos, pos + 1, str));
        }
        return ans;
    }

    public:
    SearchStringGB2312InputIter(
            IterT beginIter, IterT endIter, 
            bool str_contain_crlf, bool generate_string):
        SearchStringInputIter<IterT>(beginIter, endIter, generate_string),
        contain_crlf(str_contain_crlf) {}

    SearchStringGB2312InputIter() = default;
};

template<typename T>
SearchStringGB2312InputIter<T> gb2312Begin(
        T inputIterBegin, T inputIterEnd,
        bool str_contain_crlf, bool generate_string) 
{
    return SearchStringGB2312InputIter<T>(
            inputIterBegin, inputIterEnd, 
            str_contain_crlf, generate_string);
}
template<typename T>
SearchStringGB2312InputIter<T> gb2312End(T inputIter, T inputIterEnd) {return SearchStringGB2312InputIter<T>();}

#endif // _SEARCH_STRING_GB2312_

