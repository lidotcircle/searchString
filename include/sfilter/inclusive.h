#ifndef _STRING_SFILTER_INCLUSIVE_H_
#define _STRING_SFILTER_INCLUSIVE_H_

#include "string_filter.h"
#include <aho_corasick.hpp>


class InclusiveFilter: public StringFilter {
    private:
        bool strict;
        aho_corasick::trie trie;

    public:
        InclusiveFilter(const std::string& file, bool strict);
        int filter(const std::string&) const override;
        ~InclusiveFilter() = default;
};

#endif // _STRING_SFILTER_INCLUSIVE_H_
