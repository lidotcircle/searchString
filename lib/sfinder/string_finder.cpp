#include "sfinder/string_finder.h"
#include "sfilter/string_filter.h"
#include <assert.h>
using namespace std;


void StringFinder::add_filter(std::shared_ptr<StringFilter> filter) {
    this->filters.push_back(filter);
}

void StringFinder::add_mapper(std::shared_ptr<StringMapper> mapper) {
    this->mappers.push_back(mapper);
}

bool StringFinder::filter(const string& str) {
    return apply_filters(this->filters, str);
}

vector<pair<size_t,string>> StringFinder::map(const string& str) {
    return apply_mappers(this->mappers, str);
}

vector<pair<size_t,string>>& StringFinder::fetch() {
    assert(this->results.empty() && "clear the fetch result with move semantic");

    auto& pre = this->pre_fetch();
    for(auto& p : pre) {
        if (this->filter(p.second)) {
            auto ans = this->map(p.second);
            auto base = p.first;
            for(auto& q: ans) {
                q.first += base;
                this->results.push_back(std::move(q));
            }
        }
    }

    pre.clear();
    return this->results;
}

