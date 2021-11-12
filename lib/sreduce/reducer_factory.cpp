#include "sreduce/reducer_factory.h"
#include "sreduce/identical.h"
#include "sreduce/top_n.h"
#include "sreduce/last_n.h"
#include "sreduce/least_repeated_n.h"
#include "sreduce/most_repeated_n.h"
#include "sreduce/reverse.h"
#include "sreduce/sort_by_length.h"
#include <exception>
#include <stdexcept>
using namespace std;


std::unique_ptr<StringReducer> create_identical(const std::string&) {
    return unique_ptr<StringReducer>(new IdenticalReducer());
}
std::unique_ptr<StringReducer> create_top_n(const std::string& n) {
    size_t pos = 0;
    int nx = std::stoi(n, &pos);
    if (pos != n.size() || nx <= 0)
        throw std::runtime_error("please specify a correct positive integer");

    return unique_ptr<StringReducer>(new TopNReducer(nx));
}
std::unique_ptr<StringReducer> create_last_n(const std::string& n) {
    size_t pos = 0;
    int nx = std::stoi(n, &pos);
    if (pos != n.size() || nx <= 0)
        throw std::runtime_error("please specify a correct positive integer");

    return unique_ptr<StringReducer>(new LastNReducer(nx));
}
std::unique_ptr<StringReducer> create_least_rep(const std::string& n) {
    size_t pos = 0;
    int nx = std::stoi(n, &pos);
    if (pos != n.size() || nx <= 0)
        throw std::runtime_error("please specify a correct positive integer");

    return unique_ptr<StringReducer>(new LeastRepeatedN(nx));
}
std::unique_ptr<StringReducer> create_most_rep(const std::string& n) {
    size_t pos = 0;
    int nx = std::stoi(n, &pos);
    if (pos != n.size() || nx <= 0)
        throw std::runtime_error("please specify a correct positive integer");

    return unique_ptr<StringReducer>(new MostRepeatedN(nx));
}
std::unique_ptr<StringReducer> create_reverse(const std::string&) {
    return unique_ptr<StringReducer>(new ReverseReducer());
}
std::unique_ptr<StringReducer> create_sortbylen(const std::string& param) {
    bool ascend = false;
    if (param == "ascend") {
        ascend = true;
    } else if (!param.empty()) {
        throw std::runtime_error("Invalid parameter for sortbylen reducer: " + param);
    }

    return unique_ptr<StringReducer>(new SortByLength(ascend));
}

namespace ReducerFactory {

static map<string,map<string,string>> s_reducers;
static map<string,map<string,pair<string,create_reducer_func_t>>> s_reducer_funcs = {
    { "ascii", 
        {
            { "iden",   make_pair("maximum length by truncating string", create_identical) },
            { "topn",   make_pair("top n pairs",               create_top_n) },
            { "lastn",  make_pair("last n pairs",              create_last_n) },
            { "least_rep",  make_pair("least repeated n",      create_least_rep) },
            { "most_rep",   make_pair("most repeated n",       create_most_rep) },
            { "reverse",    make_pair("reverse",               create_reverse) },
            { "sortbylen",  make_pair("sort by string length, [:ascend]", create_sortbylen) },
        }
    },
    { "gb2312",
        {
            { "iden", make_pair("maximum length by truncating string", create_identical) },
            { "topn",   make_pair("top n pairs",               create_top_n) },
            { "lastn",  make_pair("last n pairs",              create_last_n) },
            { "least_rep",  make_pair("least repeated n",      create_least_rep) },
            { "most_rep",   make_pair("most repeated n",       create_most_rep) },
            { "reverse",    make_pair("reverse",               create_reverse) },
            { "sortbylen",  make_pair("sort by string length, [:ascend]", create_sortbylen) },
        }
    }
};

std::unique_ptr<StringReducer>
create(const std::string& encoding, const std::string &reducer_expr) {
    if (s_reducer_funcs.find(encoding) == s_reducer_funcs.end())
        throw runtime_error("unsupport encoding '" + encoding + "'");

    string reducer = reducer_expr;
    string reducer_params;
    if (reducer_expr.find(":") != std::string::npos) {
        auto pos = reducer_expr.find(":");
        reducer = reducer_expr.substr(0, pos);
        reducer_params = reducer_expr.substr(pos + 1);
    }

    auto& es = s_reducer_funcs[encoding];
    if (es.find(reducer) == es.end())
        throw runtime_error(reducer + " is not supported by " + encoding);

    auto& entry = es[reducer];
    return entry.second(reducer_params);
}

static int vn = 0, vm = 1;
const map<string,map<string,string>>&
get_supported_reducers() {
    if (vn == vm)
        return s_reducers;

    s_reducers.clear();
    for (auto& e : s_reducer_funcs) {
        auto& encoding = e.first;
        auto& es = e.second;
        for (auto& ee : es) {
            auto& reducer = ee.first;
            auto& entry = ee.second;
            auto& desc = entry.first;
            s_reducers[encoding][reducer] = desc;
        }
    }
    vn = vm;
    return s_reducers;
}

void
register_reducer(const std::string& encoding,
                const std::string& reducer_type,
                const std::string& description,
                create_reducer_func_t factory)
{
    if (s_reducer_funcs.find(encoding) == s_reducer_funcs.end())
        s_reducer_funcs[encoding] = map<string,pair<string,create_reducer_func_t>>();

    auto& es = s_reducer_funcs[encoding];
    if (es.find(reducer_type) != es.end())
        throw runtime_error(reducer_type + " is already registered");

    es[reducer_type] = make_pair(description, factory);
    vm += 1;
}

}
