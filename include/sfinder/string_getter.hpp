#include "string_finder.h"
#include "../utils.hpp"
#include "../optional.hpp"
#include <type_traits>
#include <exception>


template<typename Iter,typename Finder>
class StringGetter {
private:
    static_assert(is_char_input_iterator_v<Iter>, "require a char input interator");
    static_assert(std::is_base_of<StringFinder,Finder>::value, "rquire a class which derive StringFinder");
    std::vector<std::pair<size_t,std::string>> outputs;
    std::shared_ptr<StringFinder> finder;
    Iter char_begin, char_end;
    void get_at_least_one_until_end();

    class StringGetterIter
    {
    public:
        typedef size_t difference_type;
        typedef std::pair<size_t,std::string> value_type;
        typedef value_type &reference;
        typedef value_type *pointer;
        typedef std::input_iterator_tag iterator_catogory;

    private:
        StringGetter& getter;
        bool is_end;
        bool is_proxy;
        Optional<std::pair<size_t,std::string>> proxy_output;
        StringGetterIter(value_type val);
        bool at_end();

    public:
        StringGetterIter() = delete;
        StringGetterIter(StringGetter& getter, bool end);
        StringGetterIter& operator++();
        StringGetterIter  operator++(int);
        reference         operator*();
        pointer           operator->();
        bool operator==(const StringGetterIter&) const;
        bool operator!=(const StringGetterIter&) const;
    };

protected:
    friend class StringGetterIter;
    bool empty() const;
    std::pair<size_t,std::string>& top();
    void pop();

public:
    StringGetter() = delete;
    StringGetter(Iter begin, Iter end);

    void add_filter(std::shared_ptr<StringFilter> filter);
    void add_mapper(std::shared_ptr<StringMapper> mapper);

    StringGetterIter begin();
    StringGetterIter end();
};

template<typename It, typename Ft>
void StringGetter<It,Ft>::get_at_least_one_until_end() {
    for (;this->char_begin!=this->char_end && this->outputs.empty();this->char_begin++) {
        auto c = *this->char_begin;
        this->finder->feed_char(c);
        auto all = std::move(this->finder->fetch());
        for(auto& vx: all) this->outputs.push_back(std::move(vx));
    }

    if (this->char_begin==this->char_end) {
        this->finder->feed_end();
        auto all = std::move(this->finder->fetch());
        for(auto& vx: all) this->outputs.push_back(std::move(vx));
    }
}

template<typename It, typename Ft>
bool StringGetter<It,Ft>::empty() const {
    if (!this->outputs.empty())
        return false;

    auto ncthis = const_cast<StringGetter*>(this);
    ncthis->get_at_least_one_util_end();
    return this->outputs.empty();
}

template<typename It, typename Ft>
std::pair<size_t,std::string>& StringGetter<It,Ft>::top() {
    return this->outputs.front();
}

template<typename It, typename Ft>
void StringGetter<It,Ft>::pop() {
    this->outputs.erase(this->outputs.begin());
    this->get_at_least_one_until_end();
}

template<typename It, typename Ft>
StringGetter<It,Ft>::StringGetter(It begin, It end):
    finder(new Ft(), std::default_delete<Ft>()),
    char_begin(std::forward<It>(begin)),
    char_end(std::forward<It>(end)) {}

template<typename It, typename Ft>
void StringGetter<It,Ft>::add_filter(std::shared_ptr<StringFilter> filter) {
    this->finder->add_filter(filter);
}

template<typename It, typename Ft>
void StringGetter<It,Ft>::add_mapper(std::shared_ptr<StringMapper> mapper) {
    this->finder->add_mapper(mapper);
}

template<typename It, typename Ft>
typename StringGetter<It,Ft>::StringGetterIter StringGetter<It,Ft>::begin() {
    return StringGetterIter(*this, false);
}

template<typename It, typename Ft>
typename StringGetter<It,Ft>::StringGetterIter StringGetter<It,Ft>::end() {
    return StringGetterIter(*this, true);
}

template<typename It, typename Ft>
StringGetter<It,Ft>::StringGetterIter::StringGetterIter(std::pair<size_t,std::string> val):
    is_end(false), is_proxy(true), proxy_output(std::move(val))
{
}

template<typename It, typename Ft>
bool StringGetter<It,Ft>::StringGetterIter::at_end()
{
    return this->is_end || this->getter.empty();
}

template<typename It, typename Ft>
StringGetter<It,Ft>::StringGetterIter::StringGetterIter(StringGetter& getter, bool end):
    getter(getter), is_end(end), is_proxy(false)
{
    if (!end) getter.get_at_least_one_util_end();
}

template<typename It, typename Ft>
typename StringGetter<It,Ft>::StringGetterIter& StringGetter<It,Ft>::StringGetterIter::operator++() {
    assert(!this->is_proxy && "increasing a proxy iterator");
    assert(!this->is_end && "increasing a definitely empty iterator");
    if (this->getter.empty())
        throw std::runtime_error("increasing iterator in the end");

    this->getter.pop();
    return *this;
}

template<typename It, typename Ft>
typename StringGetter<It,Ft>::StringGetterIter StringGetter<It,Ft>::StringGetterIter::operator++(int) {
    auto val = std::move(this->getter.top());

    this->getter.pop();
    return StringGetterIter(val);
}

template<typename It, typename Ft>
typename StringGetter<It,Ft>::StringGetterIter::reference StringGetter<It,Ft>::StringGetterIter::operator*() {
    return this->getter.top();
}

template<typename It, typename Ft>
typename StringGetter<It,Ft>::StringGetterIter::pointer StringGetter<It,Ft>::StringGetterIter::operator->() {
    return &this->getter.top();
}
 
template<typename It, typename Ft>
bool StringGetter<It,Ft>::StringGetterIter::operator==(const StringGetterIter& oth) const {
    if (this == &oth) return true;

    if (&this->getter != &oth.getter) return false;
    return this->at_end() && oth.at_end();
}
 
template<typename It, typename Ft>
bool StringGetter<It,Ft>::StringGetterIter::operator!=(const StringGetterIter& oth) const {
    return !this->operator==(oth);
}


template<typename Ft, typename It>
StringGetter<It,Ft> make_string_getter(It begin, It end) 
{
    return StringGetter<It,Ft>(begin, end);
}


#include "string_finder_ascii.h"
#include "string_finder_gb2312.h"
extern template class StringGetter<char*, StringFinderASCII>;
extern template class StringGetter<char*, StringFinderGB2312>;

