#ifndef _SIMPLE_ARRAY_HPP_
#define _SIMPLE_ARRAY_HPP_

#include <stdlib.h>


template<typename T, size_t N>
class simple_array {
private:
    T data[N];

public:
    constexpr simple_array(): data{ T() } {}

    constexpr T& operator[](size_t index) {
        return data[index];
    }

    constexpr const T& operator[](size_t index) const {
        return data[index];
    }

    constexpr void set_at(size_t index, T value) {
        data[index] = value;
    }

    constexpr T get_at(size_t index) const {
        return data[index];
    }
};

#endif // _SIMPLE_ARRAY_HPP_
