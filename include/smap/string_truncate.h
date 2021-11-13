#ifndef _SMAP_TRUNCATE_H_
#define _SMAP_TRUNCATE_H_

#include "string_mapper.h"

class TruncateMapper: public StringMapper {
    private:
        size_t len;

    public:
        TruncateMapper(size_t max_len);
        ~TruncateMapper() = default;

        virtual std::vector<std::pair<size_t,std::string>> map(const std::string& str) const override;

        static const std::vector<int> register_handles;
};


#endif // _SMAP_TRUNCATE_H_
