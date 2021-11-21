#ifndef _STRING_SGETTER_GETTER_FACTORY_HPP_
#define _STRING_SGETTER_GETTER_FACTORY_HPP_

#include <string>
#include <stdexcept>
#include <memory>
#include "../sfinder/string_finder_ascii.h"
#include "../sfinder/string_finder_gb2312.h"
#include "../sfinder/string_finder_utf8.h"
#include "../sfinder/string_finder_utf16.h"
#include "string_getter_base.h"
#include "string_getter_by_finder.hpp"
#include "string_getter_by_reducer.h"
#include "../sfilter/filter_factory.h"
#include "../smap/mapper_factory.h"
#include "../sreduce/reducer_factory.h"


namespace GetterFactory {

template<typename Iter>
auto create(const std::string& encoding, const std::string& param, Iter begin, Iter end) {
    if (encoding == "ascii") {
        return make_string_getter<StringFinderASCII>(begin, end);
    } else if (encoding == "gb2312") {
        return make_string_getter<StringFinderGB2312>(begin, end);
    } else if (encoding == "utf8") {
        return make_string_getter<StringFinderUTF8>(begin, end);
    } else if (encoding == "utf16") {
        return make_string_getter<StringFinderUTF16>(begin, end);
    } else {
        throw std::runtime_error("encoding not supported");
    }
}

std::unique_ptr<StringGetterBase>
create(
        std::unique_ptr<StringGetterBase> getter,
        std::unique_ptr<StringReducer> reducer);

template<typename Iter>
auto create_by_exprs(const std::string& encoding, const std::vector<std::string>& exprs, Iter begin, Iter end) {
    auto getter = create(encoding, "", begin, end);
    auto _encoding = encoding;

    for(auto& expr : exprs) {
        if (expr.size() < 2) {
            throw std::runtime_error("expr is empty");
        }
        char op = expr[0];
        std::string param = expr.substr(1);
        switch (op) {
            case 'f':
                getter->add_filter(FilterFactory::create(_encoding, param));
                break;
            case 'm':
                getter->add_mapper(MapperFactory::create(_encoding, param));
                break;
            case 'r': 
            {
                if (param.find_last_of("#") != std::string::npos) {
                    auto pos = param.find_last_of("#");
                    _encoding = param.substr(pos + 1);
                    param = param.substr(0, pos);
                }
                getter = make_string_getter(std::move(getter), ReducerFactory::create(_encoding, param));
            }  break;
            default:
                throw std::runtime_error("expr '" + expr + "' not supported");
        }
    }

    return getter;
}

}

#endif // _STRING_SGETTER_GETTER_FACTORY_HPP_
