#ifndef _STRING_FILTER_REGEX_HPP
#define _STRING_FILTER_REGEX_HPP

#include "string_filter.h"
#include <regex>

class RegexFilter: public StringFilter {
    private:
        std::regex m_regex;
        std::string m_encoding;

    public:
        RegexFilter(const std::string& encoding, const std::string& regex);
        int filter(const std::string&) const override;

        static const std::vector<int> register_handles;
};

#endif // _STRING_FILTER_REGEX_HPP
