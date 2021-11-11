#ifndef _SEARCH_STRING_UTILS_HPP_
#define _SEARCH_STRING_UTILS_HPP_

#include <type_traits>
#include <iterator>


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

#endif // _SEARCH_STRING_UTILS_HPP_
