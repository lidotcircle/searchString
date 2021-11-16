#include "win_process/process_map_iter.h"
#include "win_process/process_map.h"
#include <stdexcept>
using namespace std;

ProcessMapIter::ProcessMapIter(ProcessMap* map, size_t index): map(map),  index(index)
{
}

ProcessMapIter::~ProcessMapIter() {}

ProcessMapIter& ProcessMapIter::operator++() {
    if (index >= map->size()) {
        throw out_of_range("ProcessMapIter::operator++");
    }
    index++;
    return *this;
}

ProcessMapIter ProcessMapIter::operator++(int) {
    ProcessMapIter tmp(*this);
    operator++();
    return tmp;
}

ProcessMapIter& ProcessMapIter::operator--() {
    if (index < 0) {
        throw out_of_range("ProcessMapIter::operator--");
    }
    index--;
    return *this;
}

ProcessMapIter ProcessMapIter::operator--(int) {
    ProcessMapIter tmp(*this);
    operator--();
    return tmp;
}

ProcessMapIter& ProcessMapIter::operator+=(int n) {
    index += n;
    if (index > map->size() || index < 0) {
        throw out_of_range("ProcessMapIter::operator+=");
    }
    return *this;
}

ProcessMapIter& ProcessMapIter::operator-=(int n) {
    index -= n;
    if (index > map->size() || index < 0) {
        throw out_of_range("ProcessMapIter::operator-=");
    }
    return *this;
}

ProcessMapIter ProcessMapIter::operator+(int n) const {
    ProcessMapIter tmp(*this);
    tmp.index += n;
    if (tmp.index < 0 || tmp.index > map->size())
        throw out_of_range("ProcessMapIter::operator+");
    return tmp;
}

ProcessMapIter ProcessMapIter::operator-(int n) const {
    ProcessMapIter tmp(*this);
    tmp.index -= n;
    if (tmp.index < 0 || tmp.index > map->size())
        throw out_of_range("ProcessMapIter::operator-");
    return tmp;
}

bool ProcessMapIter::operator==(const ProcessMapIter& other) const {
    return this->map == other.map &&  index == other.index;
}

bool ProcessMapIter::operator!=(const ProcessMapIter& other) const {
    return !operator==(other);
}

MemoryValueRef ProcessMapIter::operator*() {
    // return this->map->get_at(index);
    return MemoryValueRef(map, index);
}
