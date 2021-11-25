#include "sfilter/filter_factory.h"
#include "sfilter/min_length.h"
#include "sfilter/inclusive.h"
#include "sfilter/exclusive.h"
#include "sfilter/exclusive_full.h"
#include "sfilter/generic_ngram_svm.hpp"
#include <exception>
#include <stdexcept>
using namespace std;


static void keep_initialization_procedures(int i) {
    auto hs = {
        MiniumLength::register_handles,
        InclusiveFilter::register_handles,
        ExclusiveFilter::register_handles,
        ExclusiveFullFilter::register_handles,
        GenericNGramSVMFilter<1,dlib::svm_c_trainer,dlib::radial_basis_kernel>::register_handles,
    };
    for (auto& h : hs) {
        for(auto& hh : h) {
            if (hh == i)
                return;
        }
    }
}

namespace FilterFactory {

static map<string,map<string,string>> s_filters;
static map<string,map<string,pair<string,create_filter_func_t>>> s_filter_funcs;

std::shared_ptr<StringFilter>
create(const std::string& encoding, const std::string &filter_expr) {
    if (s_filter_funcs.find(encoding) == s_filter_funcs.end())
        throw runtime_error("unsupport encoding '" + encoding + "'");

    string filter = filter_expr;
    string filter_params;
    if (filter_expr.find(":") != std::string::npos) {
        auto pos = filter_expr.find(":");
        filter = filter_expr.substr(0, pos);
        filter_params = filter_expr.substr(pos + 1);
    }

    auto& es = s_filter_funcs[encoding];
    if (es.find(filter) == es.end())
        throw runtime_error(filter + " is not supported by " + encoding);

    auto& entry = es[filter];
    return entry.second(filter_params);
}

static int vn = 0, vm = 1;
const std::map<std::string,std::map<std::string,std::string>>&
get_supported_filters() {
    if (vm == vn)
        return s_filters;

    s_filters.clear();
    for (auto& e : s_filter_funcs) {
        auto& encoding = e.first;
        auto& es = e.second;
        for (auto& ee : es) {
            auto& filter = ee.first;
            auto& entry = ee.second;
            auto& desc = entry.first;
            s_filters[encoding][filter] = desc;
        }
    }
    vn = vm;
    return s_filters;
}

int
register_filter(const std::string& encoding,
                const std::string& filter_type,
                const std::string& description,
                create_filter_func_t factory)
{
    if (s_filter_funcs.find(encoding) == s_filter_funcs.end())
        s_filter_funcs[encoding] = map<string,pair<string,create_filter_func_t>>();

    auto& es = s_filter_funcs[encoding];
    if (es.find(filter_type) != es.end())
        throw runtime_error(filter_type + " is already registered");

    es[filter_type] = make_pair(description, factory);
    keep_initialization_procedures(vm);
    return vm++;
}

}
