#ifndef _SEARCH_STRING_ASCII_
#define _SEARCH_STRING_ASCII_

#include <vector>
#include <tuple>
#include <string>
#include "search_string.hpp"


#define _IS_V_ASCII_CRLF(c)  ((c >= 0x20 && c <  0x7f) \
        || c == 0x09 || c == 0x0A || c == 0x0D)
#define _IS_V_ASCII_NOCRLF(c)  ((c >= 0x20 && c <  0x7f) || c == 0x09)


template<typename IterT>
class SearchStringASCIIInputIter: public SearchStringInputIter<IterT> {
    using output_array_t = typename SearchStringInputIter<IterT>::output_array_t;
    bool contain_crlf;

    std::string candidate;
    size_t candidate_begin;

    bool is_ascii(unsigned char c) {
        return contain_crlf ? _IS_V_ASCII_CRLF(c) : _IS_V_ASCII_NOCRLF(c);
    }

    virtual output_array_t feed_char(unsigned char c) override
    {
        output_array_t ans;
        if (this->is_ascii(c)) {
            if (this->candidate.empty())
                this->candidate_begin = this->currentPosition();
            this->candidate.push_back(c);
        } else {
            if (!this->candidate.empty())
            {
                ans.push_back(make_tuple(
                    this->candidate_begin,
                    this->currentPosition(),
                    std::move(this->candidate)));
            }
        }

        return ans;
    }

    public:
    SearchStringASCIIInputIter(
            IterT beginIter, IterT endIter, 
            bool str_contain_crlf, bool generate_string):
        SearchStringInputIter<IterT>(beginIter, endIter, generate_string),
        contain_crlf(str_contain_crlf) {}

    SearchStringASCIIInputIter() = default;
};

template<typename T>
SearchStringASCIIInputIter<T> asciiBegin(
        T inputIterBegin, T inputIterEnd,
        bool str_contain_crlf, bool generate_string) 
{
    return SearchStringASCIIInputIter<T>(
            inputIterBegin, inputIterEnd, 
            str_contain_crlf, generate_string);
}
template<typename T>
SearchStringASCIIInputIter<T> asciiEnd(T inputIter, T inputIterEnd) {return SearchStringASCIIInputIter<T>();}

#endif // _SEARCH_STRING_ASCII_

