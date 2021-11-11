#ifndef _STRING_SFINDER_GETTER_FACTORY_HPP_
#define _STRING_SFINDER_GETTER_FACTORY_HPP_

#include <string>
#include <stdexcept>
#include "string_finder_ascii.h"
#include "string_finder_gb2312.h"
#include "string_getter.hpp"


namespace FinderFactory {

template<typename Iter>
auto create(const std::string& encoding, const std::string& param, Iter begin, Iter end) {
    if (encoding == "ascii") {
        return make_string_getter<StringFinderGB2312>(begin, end);
    } else if (encoding == "gb2312") {
        return make_string_getter<StringFinderGB2312>(begin, end);
    } else {
        throw std::runtime_error("encoding not supported");
    }
}

}

#endif // _STRING_SFINDER_GETTER_FACTORY_HPP_
