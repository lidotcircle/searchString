#ifndef _STRING_GETTER_ITER_H_
#define _STRING_GETTER_ITER_H_

#include "../optional.hpp"


class StringGetterBase;
class StringGetterIter
{
public:
    typedef size_t difference_type;
    typedef std::pair<size_t,std::string> value_type;
    typedef value_type &reference;
    typedef value_type *pointer;
    typedef std::input_iterator_tag iterator_catogory;

private:
    StringGetterBase* pgetter;
    bool is_end;
    bool is_proxy;
    Optional<std::pair<size_t,std::string>> proxy_output;
    StringGetterIter(value_type val);
    bool at_end() const;

public:
    StringGetterIter() = delete;
    StringGetterIter(StringGetterBase* pgetter, bool end);
    StringGetterIter& operator++();
    StringGetterIter  operator++(int);
    reference         operator*();
    pointer           operator->();
    bool operator==(const StringGetterIter&) const;
    bool operator!=(const StringGetterIter&) const;
};

#endif // _STRING_GETTER_ITER_H_
