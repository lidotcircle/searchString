#ifndef _SIMPLE_ARCHIVE_INTEGRAL_HPP_
#define _SIMPLE_ARCHIVE_INTEGRAL_HPP_

#include <type_traits>
#include <stdlib.h>


template<typename IT,
         typename = typename std::enable_if<std::is_integral<IT>::value, void>::type>
bool writeToBuf(const IT& i, char* buf, size_t bufsize, size_t& writed) {
    size_t n = sizeof(i);
    writed = n;

    if (n > bufsize)
        return false;

    *(IT*)buf = i;
    return true;
}

template<typename IT,
         typename = typename std::enable_if<std::is_integral<IT>::value, void>::type>
bool readFromBuf(IT& i, char* buf, size_t bufsize, size_t& readed) {
    size_t n = sizeof(i);
    if (n > bufsize)
        return false;

    i = *(IT*)buf;
    readed = n;
    return true;
}

#endif // _SIMPLE_ARCHIVE_INTEGRAL_HPP_
