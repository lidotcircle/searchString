#ifndef _PROCESS_MAP_ITER_H_
#define _PROCESS_MAP_ITER_H_

#include "memory_value_ref.h"
#include <iterator>


class ProcessMap;

class ProcessMapIter {
public:
    using difference_type = std::ptrdiff_t;
    using value_type = char;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::forward_iterator_tag;

private:
    ProcessMap *map;
    int index;

public:
    ProcessMapIter(ProcessMap *map, size_t);
    ~ProcessMapIter();

    ProcessMapIter &operator++();
    ProcessMapIter operator++(int);
    ProcessMapIter &operator--();
    ProcessMapIter operator--(int);

    ProcessMapIter &operator+=(int n);
    ProcessMapIter &operator-=(int n);

    ProcessMapIter operator+(int n) const;
    ProcessMapIter operator-(int n) const;

    bool operator==(const ProcessMapIter &other) const;
    bool operator!=(const ProcessMapIter &other) const;

    // char operator*();
    MemoryValueRef operator*();
};

#endif // _PROCESS_MAP_ITER_H_
