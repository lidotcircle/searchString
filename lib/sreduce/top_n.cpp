#include "sreduce/top_n.h"
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

