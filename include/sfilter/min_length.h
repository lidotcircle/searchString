#ifndef _STRING_SFILTER_MIN_LENGTH_H_
#define _STRING_SFILTER_MIN_LENGTH_H_

#include "string_filter.h"


class MiniumLength: public StringFilter {
    private:
        size_t min;

    public:
        MiniumLength(size_t);
        int filter(const std::string&) const override;
        ~MiniumLength() = default;
};

#endif // _STRING_SFILTER_MIN_LENGTH_H_
