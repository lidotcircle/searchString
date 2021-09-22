#include "istream_iterator.h"
#include <exception>

#define BUFSIZE (1 << 16)


istream_iterator::istream_iterator():
    is_proxy(false), proxy_value(0), 
    buffer(nullptr), buflen(0) {}

istream_iterator::istream_iterator(std::istream& _stream):
    is_proxy(false), proxy_value(0),
    buffer(nullptr), buflen(0),
    stream(&_stream) { this->feed_buffer(); }

istream_iterator::istream_iterator(char value):
    is_proxy(true), proxy_value(value),
    buffer(nullptr), buflen(0) {}

istream_iterator::~istream_iterator() {
    if (this->buffer != nullptr) {
        delete[] this->buffer;
        this->buffer = nullptr;
        this->buflen = 0;
    }
}

bool istream_iterator::operator==(const istream_iterator& o) const {
    if (this == &o) return true;

    if (this->is_proxy || o.is_proxy) return false;

    return !this->buffer && !this->buffer;
}

bool istream_iterator::operator!=(const istream_iterator& o) const {
    return !(*this == o);
}

istream_iterator::value_type istream_iterator::operator*() const {
    if (this->is_proxy) return this->proxy_value;

    if (!this->buffer)
        throw std::runtime_error("access end of iterator");
    return this->buffer[this->curpos];
}

istream_iterator  istream_iterator::operator++(int) {
    istream_iterator ans(this->operator*());

    this->operator++();
    return ans;
}

istream_iterator& istream_iterator::operator++() {
    if (!this->buffer)
        throw std::runtime_error("increase end of iterator");

    this->curpos++;
    this->feed_buffer();
    return *this;
}

void istream_iterator::feed_buffer() {
    if (this->buffer && this->curpos < this->buflen)
        return;

    if (!this->buffer)
        this->buffer = new char[BUFSIZE];

    size_t n;
    if (this->stream && 
            (n = this->stream->readsome(this->buffer, BUFSIZE))) 
    {
        this->buflen = n;
        this->curpos = 0;
    } else {
        delete[] this->buffer;
        this->stream = nullptr;
    }
}

