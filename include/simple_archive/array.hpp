#ifndef _SIMPLE_ARCHIVE_ARRAY_HPP_
#define _SIMPLE_ARCHIVE_ARRAY_HPP_

#include <array>

#define ARRAY_A_MINUS_B_GE_ZERO(a, b) ((a) >= (b) ? (a) - (b) : 0)


template<size_t N, typename T>
bool writeToBuf(const std::array<T, N>& arr, char* buf, size_t bufsize, size_t& writed) {
    bool ret = true;
    for (size_t i = 0; i < N; i++) {
        size_t n = 0;
        if (!writeToBuf(arr[i], buf + writed, ARRAY_A_MINUS_B_GE_ZERO(bufsize, writed), n))
            ret = false;
        writed += n;
    }
    return ret;
}

template<size_t N, typename T>
bool readFromBuf(std::array<T, N>& arr, char* buf, size_t bufsize, size_t& readed) {
    readed = 0;
    for (size_t i = 0; i < N; i++) {
        size_t n = 0;
        if (!readFromBuf(arr[i], buf + readed, ARRAY_A_MINUS_B_GE_ZERO(bufsize, readed), n))
            return false;
        readed += n;
    }
    return true;
}

#endif // _SIMPLE_ARCHIVE_ARRAY_HPP_
