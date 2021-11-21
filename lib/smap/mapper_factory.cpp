#include "smap/mapper_factory.h"
#include "smap/string_truncate.h"
#include "smap/string_split_line.h"
#include "smap/base64.h"
#include "smap/hexadecimal.h"
#include "smap/trim.h"
#include "smap/utf16_to_utf8.h"
#include "smap/utf8_to_utf16.h"
#include "smap/utf8_to_gb2312.h"
#include "smap/gb2312_to_utf8.h"
#include <exception>
#include <stdexcept>
using namespace std;


static void keep_initialization_procedures(int i) {
    auto hs = {
        TruncateMapper::register_handles,
        SplitLineMapper::register_handles,
        Base64Mapper::register_handles,
        HexMapper::register_handles,
        TrimMapper::register_handles,
        UTF16ToUTF8Mapper::register_handles,
        UTF8ToUTF16Mapper::register_handles,
        UTF8ToGB2312Mapper::register_handles,
        GB2312ToUTF8Mapper::register_handles,
    };
    for (auto& h : hs) {
        for(auto& hh : h) {
            if (hh == i)
                return;
        }
    }
}

namespace MapperFactory {

static map<string,map<string,string>> s_mappers;
static map<string,map<string,pair<string,create_mapper_func_t>>> s_mapper_funcs;

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

int
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
    keep_initialization_procedures(vm);
    return vm++;
}

}
