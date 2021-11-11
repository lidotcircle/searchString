#ifndef _STRING_SFILTER_GB2312_FREQUENCY_H_
#define _STRING_SFILTER_GB2312_FREQUENCY_H_

#include "string_filter.h"


class GB2312Frequency: public StringFilter {
    private:
        uint8_t threshold;

    public:
        GB2312Frequency();
        GB2312Frequency(uint8_t threshold);
        int filter(const std::string&) const override;
        ~GB2312Frequency() = default;
};


#include <set>
extern const std::set<uint16_t> gb2312_table;

#endif // _STRING_SFILTER_GB2312_FREQUENCY_H_
