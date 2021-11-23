#include "process/memory_map_iter.h"
#include "process/memory_map.h"
#include <stdexcept>
using namespace std;

MemoryMapIter::MemoryMapIter(MemoryMap* map, size_t index): map(map),  index(index)
{
}

MemoryMapIter::~MemoryMapIter() {}

MemoryMapIter& MemoryMapIter::operator++() {
    if (index >= map->size()) {
        throw out_of_range("ProcessMapIter::operator++");
    }
    index++;
    return *this;
}

MemoryMapIter MemoryMapIter::operator++(int) {
    MemoryMapIter tmp(*this);
    operator++();
    return tmp;
}

MemoryMapIter& MemoryMapIter::operator--() {
    if (index < 0) {
        throw out_of_range("ProcessMapIter::operator--");
    }
    index--;
    return *this;
}

MemoryMapIter MemoryMapIter::operator--(int) {
    MemoryMapIter tmp(*this);
    operator--();
    return tmp;
}

MemoryMapIter& MemoryMapIter::operator+=(int n) {
    index += n;
    if (index > map->size() || index < 0) {
        throw out_of_range("ProcessMapIter::operator+=");
    }
    return *this;
}

MemoryMapIter& MemoryMapIter::operator-=(int n) {
    index -= n;
    if (index > map->size() || index < 0) {
        throw out_of_range("ProcessMapIter::operator-=");
    }
    return *this;
}

MemoryMapIter MemoryMapIter::operator+(int n) const {
    MemoryMapIter tmp(*this);
    tmp.index += n;
    if (tmp.index < 0 || tmp.index > map->size())
        throw out_of_range("ProcessMapIter::operator+");
    return tmp;
}

MemoryMapIter MemoryMapIter::operator-(int n) const {
    MemoryMapIter tmp(*this);
    tmp.index -= n;
    if (tmp.index < 0 || tmp.index > map->size())
        throw out_of_range("ProcessMapIter::operator-");
    return tmp;
}

bool MemoryMapIter::operator==(const MemoryMapIter& other) const {
    return this->map == other.map &&  index == other.index;
}

bool MemoryMapIter::operator!=(const MemoryMapIter& other) const {
    return !operator==(other);
}

MemoryValueRef MemoryMapIter::operator*() {
    // return this->map->get_at(index);
    return MemoryValueRef(map, index);
}
