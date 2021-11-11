#include "smap/mapper_factory.h"
#include "smap/string_truncate.h"
#include "smap/gb2312_longest_valid_substr.h"
#include "smap/string_split_line.h"
#include <exception>
#include <stdexcept>
using namespace std;


std::shared_ptr<StringMapper> create_truncate(const std::string& max_len) {
    size_t pos = 0;
    int len = std::stoi(max_len, &pos);
    if (pos != max_len.size() || len <= 0)
        throw std::runtime_error("please specify a correct positive integer");

    return shared_ptr<StringMapper>(new TruncateMapper(len));
}

std::shared_ptr<StringMapper> create_split(const std::string&) {
    return shared_ptr<StringMapper>(new SplitLineMapper());
}

std::shared_ptr<StringMapper> create_gb2312_longest_valid(const std::string&) {
    return shared_ptr<StringMapper>(new GB2312LongestValidSubstr());
}

namespace MapperFactory {

static map<string,map<string,string>> s_mappers;
static map<string,map<string,pair<string,create_mapper_func_t>>> s_mapper_funcs = {
    { "ascii", 
        {
            { "trun", make_pair("maximum length by truncating string", create_truncate) },
            { "splt", make_pair("split line by cr lf",                 create_split) },
        }
    },
    { "gb2312",
        {
            { "trun", make_pair("maximum length by truncating string", create_truncate) },
            { "splt", make_pair("split line by cr lf",                 create_split) },
            { "mval", make_pair("longest valid substr",                create_gb2312_longest_valid) },
        }
    }
};

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

static int vn = 0, vm = 1;
const map<string,map<string,string>>&
get_supported_mappers() {
    if (vn == vm)
        return s_mappers;

    s_mappers.clear();
    for (auto& e : s_mapper_funcs) {
        auto& encoding = e.first;
        auto& es = e.second;
        for (auto& ee : es) {
            auto& mapper = ee.first;
            auto& entry = ee.second;
            auto& desc = entry.first;
            s_mappers[encoding][mapper] = desc;
        }
    }
    vn = vm;
    return s_mappers;
}

void
register_mapper(const std::string& encoding,
                const std::string& mapper_type,
                const std::string& description,
                create_mapper_func_t factory)
{
    if (s_mapper_funcs.find(encoding) == s_mapper_funcs.end())
        s_mapper_funcs[encoding] = map<string,pair<string,create_mapper_func_t>>();

    auto& es = s_mapper_funcs[encoding];
    if (es.find(mapper_type) != es.end())
        throw runtime_error(mapper_type + " is already registered");

    es[mapper_type] = make_pair(description, factory);
    vm += 1;
}

}
