#ifndef _ISTREAM_ITERATOR_H_
#define _ISTREAM_ITERATOR_H_

#include <iterator>
#include <iostream>
#include <memory>


class istream_iterator {
    public:
    typedef size_t difference_type;
    typedef char   value_type;
    typedef value_type &reference;
    typedef value_type *pointer;
    typedef std::input_iterator_tag iterator_catogory;

    private:
    std::istream* stream;
    char proxy_value;
    bool is_proxy;
    char*  buffer;
    size_t buflen;
    size_t curpos;
    istream_iterator(char value);
    void feed_buffer();

    public:
    istream_iterator();
    istream_iterator(std::istream& stream);

    bool operator==(const istream_iterator&) const;
    bool operator!=(const istream_iterator&) const;

    value_type operator*() const;

    istream_iterator  operator++(int);
    istream_iterator& operator++();

    ~istream_iterator();
};

#endif // _ISTREAM_ITERATOR_H_
