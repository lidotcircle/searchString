#ifndef _MAPPER_FACTORY_H_
#define _MAPPER_FACTORY_H_

#include "string_mapper.h"
#include <memory>
#include <map>
#include <set>
#include <string>


namespace MapperFactory {

typedef std::shared_ptr<StringMapper> (*create_mapper_func_t)(const std::string& params);


std::shared_ptr<StringMapper>
create(const std::string& encoding, const std::string &mapper_expr);

const std::map<std::string,std::map<std::string,std::string>>&
get_supported_mappers();

void
register_mapper(const std::string& encoding,
                const std::string& mapper_type,
                const std::string& description,
                create_mapper_func_t factory);

}

#endif // _MAPPER_FACTORY_H_
