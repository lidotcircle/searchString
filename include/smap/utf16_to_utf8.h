#ifndef _SMAP_UTF16_TO_UTF8_H_
#define _SMAP_UTF16_TO_UTF8_H_

#include "string_mapper.h"

class UTF16ToUTF8Mapper: public StringMapper {
    public:
        virtual std::vector<std::pair<size_t,std::string>> map(const std::string& str) const override;

        static const std::vector<int> register_handles;
};

#endif // _SMAP_UTF16_TO_UTF8_H_
