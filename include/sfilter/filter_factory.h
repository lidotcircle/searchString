#ifndef _FILTER_FACTORY_H_
#define _FILTER_FACTORY_H_

#include "string_filter.h"
#include <memory>
#include <map>
#include <set>
#include <string>


namespace FilterFactory {

typedef std::shared_ptr<StringFilter> (*create_filter_func_t)(const std::string& params);


std::shared_ptr<StringFilter>
create(const std::string& encoding, const std::string &filter_expr);

const std::map<std::string,std::map<std::string,std::string>>&
get_supported_filters();

void
register_filter(const std::string& encoding,
                const std::string& filter_type,
                const std::string& description,
                create_filter_func_t factory);

}

#endif // _FILTER_FACTORY_H_
