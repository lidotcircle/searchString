#include "sreduce/string_reducer.h"
#include "sreduce/identical.h"


void IdenticalReducer::reduce(const std::pair<size_t,std::string>& pair) {
    this->saver.push_back(pair);
}

std::vector<std::pair<size_t,std::string>> IdenticalReducer::reduce_end() {
    return std::move(this->saver);
}

