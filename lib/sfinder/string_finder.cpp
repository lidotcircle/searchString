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

string StringFinder::map(const string& str) {
    auto ans = str;
    for (auto mapper : this->mappers)
        ans = mapper->map(ans);

    return ans;
}

vector<pair<size_t,string>>& StringFinder::fetch() {
    assert(this->results.empty() && "clear the fetch result with move semantic");

    auto& pre = this->pre_fetch();
    for(auto& p : pre) {
        if (this->filter(p.second)) {
            auto ans = this->map(p.second);
            this->results.push_back(make_pair(p.first, ans));
        }
    }

    pre.clear();
    return this->results;
}

