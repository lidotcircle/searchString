#ifndef _SIMPLE_ARCHIVE_HPP_
#define _SIMPLE_ARCHIVE_HPP_

#include <map>
#include <type_traits>

/**
 * @brief Simple functions for serialization and deserialization
 *
 * @function template
 *    bool writeToBuf(T&, char* buf, size_t bufsize, size_t& writed);
 *        @tparam T The type of the data to serialize.
 *        @param buf storage for storing the serialized data.
 *        @param bufsize size of the storage.
 *        @param writed the number of bytes would write to the storage,
 *               even if the storage is not enough, it will return correct value.
 *        @return true if the data is serialized successfully, false otherwise.
 *
 * @function template
 *    bool readFromBuf(T&, char* buf, size_t bufsize, size_t& readed);
 *        @tparam T The type of the data to deserialize.
 *        @param buf storage for storing the serialized data.
 *        @param bufsize size of the storage.
 *        @param readed the number of bytes would read from the storage,
 *        @return true if the data is deserialized successfully, false otherwise.
 *
 */


// integral
template<typename IT,
         typename = typename std::enable_if<std::is_integral<IT>::value, void>::type>
bool writeToBuf(const IT& i, char* buf, size_t bufsize, size_t& writed);
template<typename IT,
         typename = typename std::enable_if<std::is_integral<IT>::value, void>::type>
bool readFromBuf(IT& i, char* buf, size_t bufsize, size_t& readed);


// map
template<typename KT, typename VT>
bool writeToBuf(const std::map<KT,VT>& map, char* buf, size_t bufsize, size_t& writed);
template<typename KT, typename VT>
bool readFromBuf(std::map<KT,VT>& map, char* buf, size_t bufsize, size_t& readed);


// tuple
template<typename ...Types>
bool writeToBuf(const std::tuple<Types...>& tup, char* buf, size_t bufsize, size_t& writed);
template<>
bool writeToBuf<>(const std::tuple<>& tup, char* buf, size_t bufsize, size_t& writed);
template<typename ...Types>
bool readFromBuf(std::tuple<Types...>& tup, char* buf, size_t bufsize, size_t& readed);
template<>
bool readFromBuf<>(std::tuple<>& tup, char* buf, size_t bufsize, size_t& read);


// array
template<size_t N, typename T>
bool writeToBuf(const std::array<T, N>& arr, char* buf, size_t bufsize, size_t& writed);
template<size_t N, typename T>
bool readFromBuf(std::array<T, N>& arr, char* buf, size_t bufsize, size_t& readed);


#include "simple_archive/integral.hpp"
#include "simple_archive/map.hpp"
#include "simple_archive/tuple.hpp"
#include "simple_archive/array.hpp"

#endif // _SIMPLE_ARCHIVE_HPP_
