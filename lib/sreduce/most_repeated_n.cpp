#include "sreduce/most_repeated_n.h"
#include <tuple>
#include <algorithm>
using namespace std;


MostRepeatedN::MostRepeatedN(size_t n): n(n) {}

void MostRepeatedN::reduce(const pair<size_t,string>& pair) {
    if (this->saver.find(pair.second) == this->saver.end())
        this->saver[pair.second] = std::make_pair(1,pair.first);
    else
        this->saver[pair.second].first++;
}

vector<pair<size_t,string>> MostRepeatedN::reduce_end() {
    vector<tuple<size_t,string,size_t>> v;
    for (auto& p: this->saver)
        v.push_back(make_tuple(p.second.first,p.first,p.second.second));

    std::sort(v.begin(),v.end(),[](const tuple<size_t,string,size_t>& a, const tuple<size_t,string,size_t>& b) {
        return get<0>(a) > get<0>(b);
    });

    vector<pair<size_t,string>> result;
    size_t i = 0;
    for (auto& t: v) {
        if (i++ >= this->n)
            break;
        result.push_back(make_pair(get<2>(t),get<1>(t)));
    }

    this->saver.clear();
    return result;
}

