#include "sreduce/merge.h"
#include "sreduce/reducer_factory.h"
using namespace std;


void MergeReducer::reduce(const pair<size_t,string>& pair) {
    if (result.second.empty())
        result.first = pair.first;
    result.second += pair.second;
}

vector<pair<size_t,string>> MergeReducer::reduce_end() {
    return { std::move(this->result) };
}

static auto creator = [](const string&) { return unique_ptr<StringReducer>(new MergeReducer()); };
const vector<int> MergeReducer::register_handles =
{
    ReducerFactory::register_reducer("ascii",  "merge", "merge inputs from pipeline", creator),
    ReducerFactory::register_reducer("gb2312", "merge", "merge inputs from pipeline", creator),
    ReducerFactory::register_reducer("utf8", "merge", "merge inputs from pipeline", creator),
    ReducerFactory::register_reducer("utf16", "merge", "merge inputs from pipeline", creator),
};
