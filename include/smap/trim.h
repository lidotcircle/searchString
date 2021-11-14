#ifndef _SMAP_TRIM_H_
#define _SMAP_TRIM_H_

#include "string_mapper.h"

class TrimMapper: public StringMapper {
    private:
        bool trim_left;
        bool trim_right;

    public:
        TrimMapper(bool left, bool right);
        ~TrimMapper() = default;

        virtual std::vector<std::pair<size_t,std::string>> map(const std::string& str) const override;

        static const std::vector<int> register_handles;
};


#endif // _SMAP_TRIM_H_
