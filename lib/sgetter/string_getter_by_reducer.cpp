#include "sgetter/string_getter_by_reducer.h"
#include "sfilter/string_filter.h"
#include "smap/string_mapper.h"
#include <assert.h>
using namespace std;


bool StringGetterByReducer::empty() const {
    return this->outputs.empty();
}
pair<size_t,string>& StringGetterByReducer::top() {
    return this->outputs.front();
}
void StringGetterByReducer::pop() {
    assert(!this->outputs.empty());
    this->outputs.erase(this->outputs.begin());
}
void StringGetterByReducer::get_at_least_one_until_end() {
}

StringGetterByReducer::StringGetterByReducer(
            std::unique_ptr<StringGetterBase> getter,
            std::unique_ptr<StringReducer> reducer):
    getter(std::move(getter)), reducer(std::move(reducer)), dirty(false)
{
}

void StringGetterByReducer::add_filter(std::shared_ptr<StringFilter> filter) {
    assert(!this->dirty && "getter at dirty state");
    this->filters.push_back(filter);
}
void StringGetterByReducer::add_mapper(std::shared_ptr<StringMapper> mapper) {
    assert(!this->dirty && "getter at dirty state");
    this->mappers.push_back(mapper);
}

StringGetterIter StringGetterByReducer::begin() {
    assert(!this->dirty && "getter at dirty state");
    for (auto& sv: *this->getter.get()) {
        if (!apply_filters(this->filters, sv.second))
            continue;

        auto rs = apply_mappers(this->mappers, sv.second);

        for(auto& r: rs) {
            r.first += sv.first;
            this->reducer->reduce(r);
        }
    }

    this->outputs = this->reducer->reduce_end();
    this->dirty = true;
    return StringGetterIter(this, false);
}

StringGetterIter StringGetterByReducer::end() {
    return StringGetterIter(this, true);
}

std::unique_ptr<StringGetterBase>
make_string_getter( 
        std::unique_ptr<StringGetterBase> getter,
        std::unique_ptr<StringReducer> reducer)
{
    return std::unique_ptr<StringGetterBase>(
            new StringGetterByReducer(std::move(getter), std::move(reducer)));
}

