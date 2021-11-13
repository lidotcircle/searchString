#include "sreduce/reverse.h"
#include "sreduce/reducer_factory.h"
using namespace std;


void ReverseReducer::reduce(const pair<size_t,string>& pair) {
    this->result.insert(this->result.begin(), pair);
}

vector<pair<size_t,string>> ReverseReducer::reduce_end() {
    return std::move(this->result);
}

static auto name = "reverse";
static auto description = "reverse order";
static auto creator = [](const string&) { return unique_ptr<StringReducer>(new ReverseReducer()); };
const vector<int> ReverseReducer::register_handles =
{
    ReducerFactory::register_reducer("ascii",  name, description, creator),
    ReducerFactory::register_reducer("gb2312", name, description, creator),
};
