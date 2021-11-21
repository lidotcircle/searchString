#ifndef _SMAP_GB2312_TO_UTF8_H_
#define _SMAP_GB2312_TO_UTF8_H_

#include "string_mapper.h"

class GB2312ToUTF8Mapper: public StringMapper {
    public:
        virtual std::vector<std::pair<size_t,std::string>> map(const std::string& str) const override;

        static const std::vector<int> register_handles;
};

#endif // _SMAP_GB2312_TO_UTF8_H_
