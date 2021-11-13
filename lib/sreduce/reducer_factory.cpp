#include "sreduce/reducer_factory.h"
#include <exception>
#include <stdexcept>
#include "sreduce/identical.h"
#include "sreduce/last_n.h"
#include "sreduce/least_repeated_n.h"
#include "sreduce/merge.h"
#include "sreduce/most_repeated_n.h"
#include "sreduce/reverse.h"
#include "sreduce/sort_by_length.h"
#include "sreduce/top_n.h"
using namespace std;

static void keep_initialization_procedures(int i) {
    auto hs = {
        IdenticalReducer::register_handles,
        LastNReducer::register_handles,
        LeastRepeatedN::register_handles,
        MergeReducer::register_handles,
        MostRepeatedN::register_handles,
        ReverseReducer::register_handles,
        SortByLength::register_handles,
        TopNReducer::register_handles,
    };
    for (auto& h : hs) {
        for(auto& hh : h) {
            if (hh == i)
                return;
        }
    }
}

namespace ReducerFactory {

static map<string,map<string,string>> s_reducers;
static map<string,map<string,pair<string,create_reducer_func_t>>> s_reducer_funcs;

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

int
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
    keep_initialization_procedures(vm);
    return vm++;
}

}
