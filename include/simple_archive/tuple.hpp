#ifndef _SIMPLE_ARCHIVE_TUPLE_HPP_
#define _SIMPLE_ARCHIVE_TUPLE_HPP_

#include <tuple>

#define TUPLE_MIN_AB(a, b) ((a) < (b) ? (a) : (b))


template<size_t N, typename ...Types>
bool writeToBuf_tuple(const std::tuple<Types...>& tup, char* buf, size_t bufsize, size_t& writed) {
    bool ret = true;
    if (!writeToBuf(std::get<N>(tup), buf, bufsize, writed))
        ret = false;

    constexpr size_t sn = std::tuple_size<std::tuple<Types...>>::value;
    if (N + 1 == sn)
        return ret;

    size_t n = 0;
    if (!writeToBuf_tuple<TUPLE_MIN_AB(N+1,sn-1)>(tup, buf + writed, bufsize - writed, n))
        ret = false;

    writed += n;
    return ret;
}
template<typename ...Types>
bool writeToBuf(const std::tuple<Types...>& tup, char* buf, size_t bufsize, size_t& writed) {
    return writeToBuf_tuple<0>(tup, buf, bufsize, writed);
}
template<>
bool writeToBuf<>(const std::tuple<>& tup, char* buf, size_t bufsize, size_t& writed);

template<size_t N, typename ...Types>
bool readFromBuf_tuple(std::tuple<Types...>& tup, char* buf, size_t bufsize, size_t& read) {
    if(!readFromBuf(std::get<N>(tup), buf, bufsize, read))
        return false;

    constexpr size_t sn = std::tuple_size<std::tuple<Types...>>::value;
    if (N + 1 == sn)
        return true;

    size_t n = 0;
    if (!readFromBuf_tuple<TUPLE_MIN_AB(N+1,sn-1)>(tup, buf + read, bufsize - read, n))
        return false;

    read += n;
    return true;
}
template<typename ...Types>
bool readFromBuf(std::tuple<Types...>& tup, char* buf, size_t bufsize, size_t& readed) {
    readed = 0;
    return readFromBuf_tuple<0>(tup, buf, bufsize, readed);
}
template<>
bool readFromBuf<>(std::tuple<>& tup, char* buf, size_t bufsize, size_t& read);

#endif // _SIMPLE_ARCHIVE_TUPLE_HPP_
