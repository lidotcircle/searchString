#ifndef _STRING_SGETTER_GETTER_BY_FINDER_HPP_
#define _STRING_SGETTER_GETTER_BY_FINDER_HPP_

#include "../utils.hpp"
#include "../sfinder/string_finder.h"
#include "../sfinder/finder_factory.h"
#include "string_getter_base.h"
#include <type_traits>
#include <assert.h>


template<typename Iter>
class StringGetter: public StringGetterBase {
private:
    static_assert(is_char_input_iterator_v<Iter>, "require a char input interator");
    std::unique_ptr<StringFinder> finder;
    std::vector<std::pair<size_t,std::string>> outputs;
    Iter char_begin, char_end;
    bool has_feed_end;

protected:
    virtual bool empty() const override;
    virtual std::pair<size_t,std::string>& top() override;
    virtual void pop() override;
    virtual void get_at_least_one_until_end() override;

public:
    StringGetter() = delete;
    StringGetter(Iter begin, Iter end, const std::string& encoding, const std::string& params);

    virtual void add_filter(std::shared_ptr<StringFilter> filter) override;
    virtual void add_mapper(std::shared_ptr<StringMapper> mapper) override;

    virtual ~StringGetter() = default;
};

template<typename It>
void StringGetter<It>::get_at_least_one_until_end() {
    for (;this->char_begin!=this->char_end && this->outputs.empty();this->char_begin++) {
        auto c = *this->char_begin;
        this->finder->feed_char(c);
        auto all = std::move(this->finder->fetch());
        for(auto& vx: all) this->outputs.push_back(std::move(vx));
    }

    if (this->char_begin == this->char_end && !this->has_feed_end) {
        this->finder->feed_end();
        this->has_feed_end = true;
        auto all = std::move(this->finder->fetch());
        for(auto& vx: all) this->outputs.push_back(std::move(vx));
    }
}

template<typename It>
bool StringGetter<It>::empty() const {
    if (!this->outputs.empty())
        return false;

    auto ncthis = const_cast<StringGetter*>(this);
    ncthis->get_at_least_one_until_end();
    return this->outputs.empty();
}

template<typename It>
std::pair<size_t,std::string>& StringGetter<It>::top() {
    return this->outputs.front();
}

template<typename It>
void StringGetter<It>::pop() {
    assert(this->outputs.size() > 0);
    this->outputs.erase(this->outputs.begin());
    this->get_at_least_one_until_end();
}

template<typename It>
StringGetter<It>::StringGetter(It begin, It end, const std::string& encoding, const std::string& params):
    finder(FinderFactory::create(encoding, params)),
    char_begin(std::forward<It>(begin)),
    char_end(std::forward<It>(end)),
    has_feed_end(false) {}

template<typename It>
void StringGetter<It>::add_filter(std::shared_ptr<StringFilter> filter) {
    this->finder->add_filter(filter);
}

template<typename It>
void StringGetter<It>::add_mapper(std::shared_ptr<StringMapper> mapper) {
    this->finder->add_mapper(mapper);
}

template<typename It>
auto make_string_getter(It begin, It end, const std::string& encoding, const std::string& params)
{
    return std::unique_ptr<StringGetterBase>(new StringGetter<It>(begin, end, encoding, params));
}

#endif // _STRING_SGETTER_GETTER_BY_FINDER_HPP_
