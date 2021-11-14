#include "sreduce/sort_by_length.h"
#include "sreduce/reducer_factory.h"
#include <algorithm>
#include <stdexcept>
using namespace std;


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

static auto name = "sortbylen";
static auto description = "sort by string length, [:ascend]";
static auto creator = [](const string& param) {
    bool ascend = false;
    if (param == "ascend") {
        ascend = true;
    } else if (!param.empty()) {
        throw std::runtime_error("Invalid parameter for sortbylen reducer: " + param);
    }

    return unique_ptr<StringReducer>(new SortByLength(ascend));
};
const vector<int> SortByLength::register_handles =
{
    ReducerFactory::register_reducer("ascii",  name, description, creator),
    ReducerFactory::register_reducer("gb2312", name, description, creator),
};
