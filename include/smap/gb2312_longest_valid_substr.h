#ifndef _SMAP_GB2312_LONGEST_VALID_SUBSTR_H_
#define _SMAP_GB2312_LONGEST_VALID_SUBSTR_H_

#include "string_mapper.h"

class GB2312LongestValidSubstr : public StringMapper {
    public:
        GB2312LongestValidSubstr() = default;
        ~GB2312LongestValidSubstr() = default;

        virtual std::vector<std::pair<size_t,std::string>> map(const std::string& str) const override;
};


#endif // _SMAP_GB2312_LONGEST_VALID_SUBSTR_H_
