#include "sfilter/gb2312_frequency.h"
#include "utils.h"
#include <assert.h>


GB2312Frequency::GB2312Frequency(): threshold(50) {}
GB2312Frequency::GB2312Frequency(uint8_t threshold): threshold(threshold) {assert(threshold <= 100);}

int GB2312Frequency::filter(const std::string& str) const
{
    auto code = gb2312str2twobytes(str);
    uint32_t n = 0, m = code.size();

    for (auto c: code)
        if (gb2312_table.find(c) != gb2312_table.end())
            ++n;

    if (n * 100 < m * threshold)
        return -1;
    else
        return 0;
}

