#include "sfinder/string_finder.h"
#include <assert.h>
using namespace std;


void StringFinder::add_filter(std::shared_ptr<StringFilter> filter) {
    this->filters.push_back(filter);
}

void StringFinder::add_mapper(std::shared_ptr<StringMapper> mapper) {
    this->mappers.push_back(mapper);
}

bool StringFinder::filter(const string& str) {
    for (auto filter : this->filters) {
        auto result = filter->filter(str);
        if (result < 0)
            return false;
        if (result > 0)
            return true;
    }
    return true;
}

vector<pair<size_t,string>> StringFinder::map(const string& str) {
    vector<pair<size_t,string>> result, interm;
    result.push_back(make_pair(0, str));

    for (auto mapper : this->mappers) {
        std::swap(result, interm);

        for (auto& pair : interm) {
            auto mapped = mapper->map(pair.second);

            for (auto& m : mapped) {
                m.first += pair.first;
                result.push_back(std::move(m));
            }
        }
    }

    return result;
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

