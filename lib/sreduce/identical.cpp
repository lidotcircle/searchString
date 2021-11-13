#include "sreduce/reducer_factory.h"
#include "sreduce/identical.h"
using namespace std;


void IdenticalReducer::reduce(const std::pair<size_t,std::string>& pair) {
    this->saver.push_back(pair);
}

std::vector<std::pair<size_t,std::string>> IdenticalReducer::reduce_end() {
    return std::move(this->saver);
}

static auto name = "iden";
static auto description = "identical transform";
static auto creator = [](const string&) { return unique_ptr<StringReducer>(new IdenticalReducer()); };
const vector<int> IdenticalReducer::register_handles =
{
    ReducerFactory::register_reducer("ascii",  name, description, creator),
    ReducerFactory::register_reducer("gb2312", name, description, creator),
};
