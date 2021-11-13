#ifndef _SIMPLE_ARCHIVE_MAP_HPP_
#define _SIMPLE_ARCHIVE_MAP_HPP_

#include <map>
#include <stdlib.h>

#define MAP_A_MINUS_B_GE_ZERO(a, b) ((a) >= (b) ? (a) - (b) : 0)


template<typename KT, typename VT>
bool writeToBuf(const std::map<KT,VT>& map, char* buf, size_t bufsize, size_t& writed) {
    bool ret = true;
    size_t i = sizeof(size_t);
    if (i <= bufsize)
        *((size_t*)buf) = map.size();

    for (auto& kv: map) {
        size_t n = 0;
        if (!writeToBuf(kv.first, buf + i, MAP_A_MINUS_B_GE_ZERO(bufsize, i), n))
            ret = false;
        i += n;

        if (!writeToBuf(kv.second, buf + i, MAP_A_MINUS_B_GE_ZERO(bufsize, i), n))
            ret = false;
        i += n;
    }

    writed = i;
    return ret;
}

template<typename KT, typename VT>
bool readFromBuf(std::map<KT,VT>& map, char* buf, size_t bufsize, size_t& readed) {
    map.clear();
    readed = 0;
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

    readed = i;
    return true;
}

#endif // _SIMPLE_ARCHIVE_MAP_HPP_
