#ifndef _SEARCH_STRING_UTILS_HPP_
#define _SEARCH_STRING_UTILS_HPP_

#include <type_traits>
#include <iterator>
#include <stdexcept>


template<typename Iter>
struct is_char_input_iterator_struct {
private:
    using category = typename std::iterator_traits<Iter>::iterator_category;
    using value_type = typename std::iterator_traits<Iter>::value_type;

public:
    static constexpr bool value =
        std::is_convertible<category, std::input_iterator_tag>::value &&
        std::is_integral<value_type>::value &&
        sizeof(value_type) == 1;
};
template<typename Iter>
constexpr bool is_char_input_iterator_v = is_char_input_iterator_struct<Iter>::value;


template<size_t N_BIT>
class simple_bitset {
private:
    constexpr static size_t N_UINT8 = (N_BIT + 7 ) / 8;
    uint8_t data[N_UINT8];

public:
    constexpr simple_bitset(): data()
    {
        for (size_t i = 0; i < N_UINT8; i++)
            data[i] = 0;
    }

    constexpr void set(size_t pos)
    {
        if (pos >= N_BIT)
            throw std::out_of_range("simple_bitset::set");

        size_t pos_byte = pos / 8;
        size_t pos_bit  = pos % 8;
        data[pos_byte] |= (1 << pos_bit);
    }

    constexpr void reset(size_t pos)
    {
        if (pos >= N_BIT)
            throw std::out_of_range("simple_bitset::reset");

        size_t pos_byte = pos / 8;
        size_t pos_bit  = pos % 8;
        data[pos_byte] &= ~(1 << pos_bit);
    }

    constexpr bool test(size_t pos) const
    {
        if (pos >= N_BIT)
            throw std::out_of_range("simple_bitset::test");

        size_t pos_byte = pos / 8;
        size_t pos_bit  = pos % 8;
        return (data[pos_byte] & (1 << pos_bit)) != 0;
    }
};

#endif // _SEARCH_STRING_UTILS_HPP_
