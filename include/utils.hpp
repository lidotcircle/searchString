#ifndef _SEARCH_STRING_UTILS_HPP_
#define _SEARCH_STRING_UTILS_HPP_

#include <type_traits>
#include <iterator>
#include <stdexcept>
#include "simple_bitset.hpp"


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

template<typename T>
using is_input_iterator_t = typename std::enable_if<
    std::is_convertible<
        typename std::iterator_traits<T>::iterator_category,
        std::input_iterator_tag>::value, 
    void>::type;

template<typename T, typename VT>
constexpr bool is_iterator_value_type_same_with_v = 
    std::is_same<typename std::iterator_traits<T>::value_type, VT>::value;
template<typename T, typename VT>
using is_iterator_value_is_same_with_t = typename std::enable_if<
    is_iterator_value_type_same_with_v<T, VT>, void>::type;

#endif // _SEARCH_STRING_UTILS_HPP_
