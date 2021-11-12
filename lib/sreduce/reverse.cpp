#include "sreduce/reverse.h"
using namespace std;


void ReverseReducer::reduce(const pair<size_t,string>& pair) {
    this->result.insert(this->result.begin(), pair);
}

vector<pair<size_t,string>> ReverseReducer::reduce_end() {
    return std::move(this->result);
}

