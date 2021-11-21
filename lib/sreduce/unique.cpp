#include "sreduce/unique.h"
#include "sreduce/reducer_factory.h"
#include <stdexcept>
using namespace std;


UniqueReducer::UniqueReducer() {}

void UniqueReducer::reduce(const pair<size_t,string>& pair) {
    if (this->getted.find(pair.second) != this->getted.end())
        return;

    this->getted.insert(pair.second);
    this->result.push_back(pair);
}

vector<pair<size_t,string>> UniqueReducer::reduce_end() {
    this->getted.clear();
    return std::move(this->result);
}

static auto name = "unique";
static auto description = "clear duplicated entries";
static auto creator = [](const string& n) {
    return unique_ptr<StringReducer>(new UniqueReducer());
};
const vector<int> UniqueReducer::register_handles =
{
    ReducerFactory::register_reducer("ascii",  name, description, creator),
    ReducerFactory::register_reducer("gb2312", name, description, creator),
    ReducerFactory::register_reducer("utf8", name, description, creator),
    ReducerFactory::register_reducer("utf16", name, description, creator),
};
