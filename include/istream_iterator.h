#ifndef _ISTREAM_ITERATOR_H_
#define _ISTREAM_ITERATOR_H_

#include <iterator>
#include <iostream>
#include <memory>
#include <optional>

class istream_iterator {
    public:
    typedef size_t difference_type;
    typedef char   value_type;
    typedef value_type &reference;
    typedef value_type *pointer;
    typedef std::input_iterator_tag iterator_catogory;

    private:
    optional<std::istream&> stream;
}

#endif // _ISTREAM_ITERATOR_H_
