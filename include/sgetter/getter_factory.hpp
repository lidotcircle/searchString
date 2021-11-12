#ifndef _STRING_SGETTER_GETTER_FACTORY_HPP_
#define _STRING_SGETTER_GETTER_FACTORY_HPP_

#include <string>
#include <stdexcept>
#include <memory>
#include "../sfinder/string_finder_ascii.h"
#include "../sfinder/string_finder_gb2312.h"
#include "string_getter_base.h"
#include "string_getter_by_finder.hpp"
#include "string_getter_by_reducer.h"


namespace GetterFactory {

template<typename Iter>
auto create(const std::string& encoding, const std::string& param, Iter begin, Iter end) {
    if (encoding == "ascii") {
        return make_string_getter<StringFinderASCII>(begin, end);
    } else if (encoding == "gb2312") {
        return make_string_getter<StringFinderGB2312>(begin, end);
    } else {
        throw std::runtime_error("encoding not supported");
    }
}

std::unique_ptr<StringGetterBase>
create(
        std::unique_ptr<StringGetterBase> getter,
        std::unique_ptr<StringReducer> reducer);

}

#endif // _STRING_SGETTER_GETTER_FACTORY_HPP_
