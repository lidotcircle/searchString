#ifndef _SMAP_SPLIT_LINE_H_
#define _SMAP_SPLIT_LINE_H_

#include "string_mapper.h"

class SplitLineMapper: public StringMapper {
    public:
        SplitLineMapper() = default;
        ~SplitLineMapper() = default;

        virtual std::vector<std::pair<size_t,std::string>> map(const std::string& str) const override;

        static const std::vector<int> register_handles;
};


#endif // _SMAP_SPLIT_LINE_H_
