#include "sreduce/most_repeated_n.h"
#include "sreduce/reducer_factory.h"
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

static auto name = "most_rep";
static auto description = "most repeated n";
static auto creator = [](const string& n) {
    size_t pos = 0;
    int nx = std::stoi(n, &pos);
    if (pos != n.size() || nx <= 0)
        throw std::runtime_error("please specify a correct positive integer");

    return unique_ptr<StringReducer>(new MostRepeatedN(nx));
};
const vector<int> MostRepeatedN::register_handles =
{
    ReducerFactory::register_reducer("ascii",  name, description, creator),
    ReducerFactory::register_reducer("gb2312", name, description, creator),
};
