#ifndef _STRING_SFILTER_EXCLUSIVE_FULL_H_
#define _STRING_SFILTER_EXCLUSIVE_FULL_H_

#include "string_filter.h"
#include <set>
#include <string>


class ExclusiveFullFilter: public StringFilter {
    private:
        std::set<std::string> exc_list;

    public:
        ExclusiveFullFilter(const std::string& file);
        int filter(const std::string&) const override;
        ~ExclusiveFullFilter() = default;
};

#endif // _STRING_SFILTER_EXCLUSIVE_FULL_H_
