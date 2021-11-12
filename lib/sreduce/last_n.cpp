#include "sreduce/last_n.h"
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

