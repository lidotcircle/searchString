#ifndef _STRING_SFILTER_EXCLUSIVE_H_
#define _STRING_SFILTER_EXCLUSIVE_H_

#include "string_filter.h"
#include <aho_corasick.hpp>
#include <vector>


class ExclusiveFilter: public StringFilter {
    private:
        aho_corasick::trie trie;

    public:
        ExclusiveFilter(const std::string& file);
        int filter(const std::string&) const override;
        ~ExclusiveFilter() = default;

        static const std::vector<int> register_handles;
};

#endif // _STRING_SFILTER_EXCLUSIVE_H_
