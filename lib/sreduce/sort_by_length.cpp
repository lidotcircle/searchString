#include "sreduce/sort_by_length.h"
#include <algorithm>


SortByLength::SortByLength(bool ascend): ascend(ascend) {}

void SortByLength::reduce(const std::pair<size_t,std::string>& pair) {
    this->result.push_back(pair);
}

std::vector<std::pair<size_t,std::string>> SortByLength::reduce_end() {
    std::sort(this->result.begin(), this->result.end(), [this](const std::pair<size_t,std::string>& a, const std::pair<size_t,std::string>& b) {
        if (this->ascend) {
            return a.second.size() < b.second.size();
        } else {
            return a.second.size() > b.second.size();
        }
    });

    return std::move(this->result);
}

