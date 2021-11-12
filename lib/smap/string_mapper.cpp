#include "smap/string_mapper.h"
using namespace std;


StringMapper::~StringMapper() {}

vector<std::pair<size_t,string>>
apply_mappers(const vector<shared_ptr<StringMapper>>& mappers, const string& str) {
    vector<pair<size_t,string>> result, interm;
    result.push_back(make_pair(0, str));

    for (auto mapper: mappers) {
        std::swap(result, interm);

        for (auto& pair : interm) {
            auto mapped = mapper->map(pair.second);

            for (auto& m : mapped) {
                m.first += pair.first;
                result.push_back(std::move(m));
            }
        }
        interm.clear();
    }

    return result;
}

