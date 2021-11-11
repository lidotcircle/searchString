#include "sfilter/string_filter_gb2312_frequency.h"
#include <assert.h>


GB2312Frequency::GB2312Frequency(): threshold(50) {}
GB2312Frequency::GB2312Frequency(uint8_t threshold): threshold(threshold) {assert(threshold <= 100);}

int GB2312Frequency::filter(const std::string& str) const
{
    return 0;
}

