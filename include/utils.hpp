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

template <typename FUNC>
struct deferred_call
{
    deferred_call(const deferred_call& that) = delete;
    deferred_call& operator=(const deferred_call& that) = delete;

    deferred_call(FUNC&& f) 
        : m_func(std::forward<FUNC>(f)), m_bOwner(true) 
    {
    }

    deferred_call(deferred_call&& that)
        : m_func(std::move(that.m_func)), m_bOwner(that.m_bOwner)
    {
        that.m_bOwner = false;
    }

    ~deferred_call()
    {
        execute();
    }

    bool cancel()
    {
        bool bWasOwner = m_bOwner;
        m_bOwner = false;
        return bWasOwner;
    }

    bool execute()
    {
        const auto bWasOwner = m_bOwner;

        if (m_bOwner)
        {
            m_bOwner = false;
            m_func();
        }

        return bWasOwner;
    }

private:
    FUNC m_func;
    bool m_bOwner;
};

template <typename F>
deferred_call<F> defer(F&& f)
{
    return deferred_call<F>(std::forward<F>(f));
}

#endif // _SEARCH_STRING_UTILS_HPP_
