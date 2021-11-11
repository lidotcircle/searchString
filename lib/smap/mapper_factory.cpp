#include "smap/mapper_factory.h"
#include <exception>
using namespace std;


namespace MapperFactory {

static map<string,set<string>> s_mappers = {};
static map<string,map<string,pair<string,create_mapper_func_t>>> s_mapper_funcs = {};

std::shared_ptr<StringMapper>
create(const std::string& encoding, const std::string &mapper_expr) {
    if (s_mapper_funcs.find(encoding) == s_mapper_funcs.end())
        throw runtime_error("unsupport encoding '" + encoding + "'");

    string mapper = mapper_expr;
    string mapper_params;
    if (mapper_expr.find(":") != std::string::npos) {
        auto pos = mapper_expr.find(":");
        mapper = mapper_expr.substr(0, pos);
        mapper_params = mapper_expr.substr(pos + 1);
    }

    auto& es = s_mapper_funcs[encoding];
    if (es.find(mapper) == es.end())
        throw runtime_error(mapper + " is not supported by " + encoding);

    auto& entry = es[mapper];
    return entry.second(mapper_params);
}

const std::map<std::string,std::set<std::string>>&
get_supported_mappers() { return s_mappers; }

void
register_mapper(const std::string& encoding,
                const std::string& mapper_type,
                const std::string& description,
                create_mapper_func_t factory)
{
    s_mappers[encoding].insert(mapper_type);

    if (s_mapper_funcs.find(encoding) == s_mapper_funcs.end())
        s_mapper_funcs[encoding] = map<string,pair<string,create_mapper_func_t>>();

    auto& es = s_mapper_funcs[encoding];
    if (es.find(mapper_type) != es.end())
        throw runtime_error(mapper_type + " is already registered");

    es[mapper_type] = make_pair(description, factory);
}

}
