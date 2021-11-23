#include "process/memory_map_collection.h"
#include <stdexcept>
using namespace std;

char MemoryMapCollection::get_at(size_t addr) const {
    for (size_t i=0;i<this->map_count();i++) {
        auto region = this->get_map(i);
        size_t base = reinterpret_cast<size_t>(region->baseaddr());
        if (base <= addr && addr < base + region->size()) {
            return region->get_at(addr - base);
        }
    }

    throw runtime_error("Address not mapped");
}

const std::shared_ptr<MemoryMap> MemoryMapCollection::get_map(size_t index) const {
    auto _this = const_cast<MemoryMapCollection*>(this);
    return _this->get_map(index);
}

void MemoryMapCollection::set_at(size_t addr, char value) {
    for (size_t i=0;i<this->map_count();i++) {
        auto region = this->get_map(i);
        size_t base = reinterpret_cast<size_t>(region->baseaddr());
        if (base <= addr && addr < base + region->size()) {
            return region->set_at(addr - base, value);
        }
    }

    throw runtime_error("Address not mapped");
}

bool MemoryMapCollection::is_valid_addr(size_t addr) {
    for (size_t i=0;i<this->map_count();i++) {
        auto region = this->get_map(i);
        size_t base = reinterpret_cast<size_t>(region->baseaddr());
        if (base <= addr && addr < base + region->size()) {
            return true;
        }
    }

    return false;
}

MemoryValueRef MemoryMapCollection::operator[] (size_t addr) {
    if (!this->is_valid_addr(addr))
        throw runtime_error("Address not mapped");

    return MemoryValueRef(this, addr);
}
