#include "sgetter/string_getter_iter.h"
#include "sgetter/string_getter_base.h"
#include <assert.h>
#include <stdexcept>
using namespace std;


StringGetterIter::StringGetterIter(std::pair<size_t,std::string> val):
    pgetter(nullptr), is_end(false), is_proxy(true), proxy_output(std::move(val))
{
}

bool StringGetterIter::at_end() const
{
    return this->is_end || this->pgetter->empty();
}

StringGetterIter::StringGetterIter(StringGetterBase* pgetter, bool end):
    pgetter(pgetter), is_end(end), is_proxy(false)
{
    if (!end) pgetter->get_at_least_one_until_end();
}

StringGetterIter& StringGetterIter::operator++() {
    assert(!this->is_proxy && "increasing a proxy iterator");
    assert(!this->is_end && "increasing a definitely empty iterator");
    if (this->pgetter->empty())
        throw std::runtime_error("increasing iterator in the end");

    this->pgetter->pop();
    return *this;
}

StringGetterIter StringGetterIter::operator++(int) {
    auto val = std::move(this->pgetter->top());

    this->pgetter->pop();
    return StringGetterIter(val);
}

StringGetterIter::reference StringGetterIter::operator*() {
    return this->pgetter->top();
}

StringGetterIter::pointer StringGetterIter::operator->() {
    return &this->pgetter->top();
}
 
bool StringGetterIter::operator==(const StringGetterIter& oth) const {
    if (this == &oth) return true;

    if (this->pgetter != oth.pgetter) return false;
    return this->at_end() && oth.at_end();
}
 
bool StringGetterIter::operator!=(const StringGetterIter& oth) const {
    return !this->operator==(oth);
}

