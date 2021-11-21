#include "sreduce/last_n.h"
#include "sreduce/reducer_factory.h"
#include <stdexcept>
using namespace std;


LastNReducer::LastNReducer(size_t n): n(n) {}

void LastNReducer::reduce(const pair<size_t,string>& pair) {
    if (n == 0) {
        return;
    }

    if (this->result.size() == this->n)
        this->result.erase(this->result.begin());

    this->result.push_back(pair);
}

vector<pair<size_t,string>> LastNReducer::reduce_end() {
    return std::move(this->result);
}

static auto name = "lastn";
static auto description = "last n pairs";
static auto creator = [](const string& n) {
    size_t pos = 0;
    int nx = std::stoi(n, &pos);
    if (pos != n.size() || nx <= 0)
        throw std::runtime_error("please specify a correct positive integer");

    return unique_ptr<StringReducer>(new LastNReducer(nx));
};
const vector<int> LastNReducer::register_handles =
{
    ReducerFactory::register_reducer("ascii",  name, description, creator),
    ReducerFactory::register_reducer("gb2312", name, description, creator),
    ReducerFactory::register_reducer("utf8", name, description, creator),
    ReducerFactory::register_reducer("utf16", name, description, creator),
};
