#ifndef _SIMPLE_ARRAY_HPP_
#define _SIMPLE_ARRAY_HPP_

#include <stdlib.h>


template<typename T, size_t N>
class SimpleArray {
private:
    T data[N];

public:
    constexpr SimpleArray(): data{ T() } {}

    constexpr T& operator[](size_t index) {
        return data[index];
    }

    constexpr const T& operator[](size_t index) const {
        return data[index];
    }
};

#endif // _SIMPLE_ARRAY_HPP_
