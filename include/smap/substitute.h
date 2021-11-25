#ifndef _SMAP_SUBSTITUTE_H_
#define _SMAP_SUBSTITUTE_H_

#include "string_mapper.h"
#include <regex>

class SubstituteMapper: public StringMapper {
    private:
        std::regex pattern;
        std::string replacement;
        std::string encoding;

    public:
        SubstituteMapper(const std::string& encoding, const std::string& pattern, const std::string& format);
        ~SubstituteMapper() = default;

        virtual std::vector<std::pair<size_t,std::string>> map(const std::string& str) const override;

        static const std::vector<int> register_handles;
};


#endif // _SMAP_SUBSTITUTE_H_
