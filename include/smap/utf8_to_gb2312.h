#ifndef _SMAP_UTF8_TO_GB2312_H_
#define _SMAP_UTF8_TO_GB2312_H_

#include "string_mapper.h"

class UTF8ToGB2312Mapper: public StringMapper {
    public:
        virtual std::vector<std::pair<size_t,std::string>> map(const std::string& str) const override;

        static const std::vector<int> register_handles;
};

#endif // _SMAP_UTF8_TO_GB2312_H_
