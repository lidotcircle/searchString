#ifndef _SIMPLE_ARCHIVE_HPP_
#define _SIMPLE_ARCHIVE_HPP_

#include <map>
#include <type_traits>

template<typename IT,
         typename = typename std::enable_if<std::is_integral<IT>::value, void>::type>
bool writeToBuf(const IT& i, char* buf, size_t bufsize, size_t& writed) {
    size_t n = sizeof(i);

    if (buf != nullptr) {
        if(n > bufsize) {
            return false;
        }
        *(IT*)buf = i;
    }

    writed = n;
    return true;
}

template<typename IT,
         typename = typename std::enable_if<std::is_integral<IT>::value, void>::type>
bool readFromBuf(IT& i, char* buf, size_t bufsize, size_t& read) {
    size_t n = sizeof(i);
    if(n > bufsize) {
        return false;
    }

    i = *(IT*)buf;
    read = n;
    return true;
}

template<typename KT, typename VT>
bool writeToBuf(const std::map<KT,VT>& map, char* buf, size_t bufsize, size_t& writed) {
    bool nowrite = buf == nullptr;
    size_t i = sizeof(size_t);
    if(!nowrite) {
        if (i > bufsize) {
            return false;
        }
        *((size_t*)buf) = map.size();
    }

    for(auto& kv: map) {
        size_t n = 0;
        if(!writeToBuf(kv.first, nowrite ? nullptr : buf + i, bufsize - i, n)) {
            return false;
        }
        i += n;
        if(!nowrite && bufsize < i) return false;

        if(!writeToBuf(kv.second, nowrite ? nullptr : buf + i, bufsize - i, n)) {
            return false;
        }
        i += n;
        if(!nowrite && bufsize < i) return false;
    }

    writed = i;
    return true;
}

template<typename KT, typename VT>
bool readFromBuf(std::map<KT,VT>& map, char* buf, size_t bufsize, size_t& read) {
    map.clear();
    size_t i = sizeof(size_t);
    if(bufsize < i) {
        return false;
    }

    size_t n = *(size_t*)buf;
    for(;n>0;n--) {
        KT k;
        VT v;
        size_t m;
        if(!readFromBuf(k, buf + i, bufsize - i, m)) {
            return false;
        }
        i += m;
        if (bufsize < i) return false;

        if(!readFromBuf(v, buf + i, bufsize - i, m)) {
            return false;
        }
        i += m;
        if (bufsize < i) return false;

        if(map.find(k) != map.end()) {
            return false;
        }
        map.insert(std::make_pair(k, v));
    }

    read = i;
    return true;
}

#define MIN_AB(a, b) ((a) < (b) ? (a) : (b))

#include <tuple>
template<size_t N, typename ...Types>
bool writeToBuf_tuple(const std::tuple<Types...>& tup, char* buf, size_t bufsize, size_t& writed) {
    if(!writeToBuf(std::get<N>(tup), buf, bufsize, writed))
        return false;

    constexpr size_t sn = std::tuple_size<std::tuple<Types...>>::value;
    if (N + 1 == sn)
        return true;

    size_t n = 0;
    if (!writeToBuf_tuple<MIN_AB(N+1,sn-1)>(tup, buf + writed, bufsize - writed, n))
        return false;

    writed += n;
    return true;
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
    if (!readFromBuf_tuple<MIN_AB(N+1,sn-1)>(tup, buf + read, bufsize - read, n))
        return false;

    read += n;
    return true;
}
template<typename ...Types>
bool readFromBuf(std::tuple<Types...>& tup, char* buf, size_t bufsize, size_t& read) {
    return readFromBuf_tuple<0>(tup, buf, bufsize, read);
}
template<>
bool readFromBuf<>(std::tuple<>& tup, char* buf, size_t bufsize, size_t& read);

#endif // _SIMPLE_ARCHIVE_HPP_
