#include "sfilter/filter_factory.h"
#include "sfilter/gb2312_frequency.h"
#include "sfilter/gb2312_svm.h"
#include "sfilter/min_length.h"
#include <exception>
#include <stdexcept>
using namespace std;


static shared_ptr<StringFilter> create_min_filter(const string& minlen) 
{
    size_t pos = 0;
    int len = std::stoi(minlen, &pos);
    if (pos != minlen.size() || len <= 0)
        throw std::runtime_error("please specify a correct positive integer");

    return shared_ptr<StringFilter>(new MiniumLength(len));
}

static shared_ptr<StringFilter> create_gb_freq_filter(const string& threshold) 
{
    int thre = 50;
    if (!threshold.empty()) {
        size_t pos = 0;
        thre = std::stoi(threshold, &pos);
        if (pos != threshold.size() || (thre < 0 && thre > 100))
            throw std::runtime_error("please specify a integer between 1 to 100");
    }

    return shared_ptr<StringFilter>(new GB2312Frequency(thre));
}

static shared_ptr<StringFilter> create_gb_svmc_filter(const string& modelpath) 
{
    return nullptr;
}


namespace FilterFactory {

static map<string,set<string>> s_filters = {
    { "ascii",  { "min" } },
    { "gb2312", { "min", "freq", "svmc" } },
};
static map<string,map<string,pair<string,create_filter_func_t>>> s_filter_funcs = {
    { "ascii",  
        {
            { "min", { "minimum length in bytes", create_min_filter } },
        }
    },
    { "gb2312",
        {
            { "min",  { "minimum length in bytes, positive integer",  create_min_filter } },
            { "freq", { "minimum frequency of character in gb2312 charset, 1 - 100", create_gb_freq_filter } },
            { "svmc", { "svm classify, model path", create_gb_svmc_filter } },
        } 
    },
};

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

const std::map<std::string,std::set<std::string>>&
get_supported_filters() { return s_filters; }

void
register_filter(const std::string& encoding,
                const std::string& filter_type,
                const std::string& description,
                create_filter_func_t factory)
{
    s_filters[encoding].insert(filter_type);

    if (s_filter_funcs.find(encoding) == s_filter_funcs.end())
        s_filter_funcs[encoding] = map<string,pair<string,create_filter_func_t>>();

    auto& es = s_filter_funcs[encoding];
    if (es.find(filter_type) != es.end())
        throw runtime_error(filter_type + " is already registered");

    es[filter_type] = make_pair(description, factory);
}

}
