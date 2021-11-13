#include "simple_archive.hpp"

template<>
bool writeToBuf<>(const std::tuple<>& tup, char* buf, size_t bufsize, size_t& writed) {
    writed = 0;
    return true;
}

template<>
bool readFromBuf<>(std::tuple<>& tup, char* buf, size_t bufsize, size_t& read) {
    read = 0;
    return true;
}

