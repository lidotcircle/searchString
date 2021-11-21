#ifndef _SMAP_UTF8_TO_UTF16_H_
#define _SMAP_UTF8_TO_UTF16_H_

#include "string_mapper.h"

class UTF8ToUTF16Mapper: public StringMapper {
    public:
        virtual std::vector<std::pair<size_t,std::string>> map(const std::string& str) const override;

        static const std::vector<int> register_handles;
};

#endif // _SMAP_UTF8_TO_UTF16_H_
