#include "sreduce/top_n.h"
#include "sreduce/reducer_factory.h"
#include <stdexcept>
using namespace std;


TopNReducer::TopNReducer(size_t n): n(n) {}

void TopNReducer::reduce(const pair<size_t,string>& pair) {
    if (this->result.size() >= this->n) {
        return;
    }

    this->result.push_back(pair);
}

vector<pair<size_t,string>> TopNReducer::reduce_end() {
    return std::move(this->result);
}

static auto name = "topn";
static auto description = "top n pairs";
static auto creator = [](const string& n) {
    size_t pos = 0;
    int nx = std::stoi(n, &pos);
    if (pos != n.size() || nx <= 0)
        throw std::runtime_error("please specify a correct positive integer");

    return unique_ptr<StringReducer>(new TopNReducer(nx));
};
const vector<int> TopNReducer::register_handles =
{
    ReducerFactory::register_reducer("ascii",  name, description, creator),
    ReducerFactory::register_reducer("gb2312", name, description, creator),
    ReducerFactory::register_reducer("utf8", name, description, creator),
    ReducerFactory::register_reducer("utf16", name, description, creator),
};
