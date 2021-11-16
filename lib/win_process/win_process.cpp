#include "win_process/win_process.h"
#include <stdexcept>
using namespace std;

char WinProcess::get_at(size_t addr) {
    for (size_t i=0;i<this->map_count();i++) {
        auto region = this->get_map(i);
        size_t base = reinterpret_cast<size_t>(region->baseaddr());
        if (base <= addr && addr < base + region->size()) {
            return region->get_at(addr - base);
        }
    }

    throw runtime_error("Address not mapped");
}

void WinProcess::set_at(size_t addr, char value) {
    for (size_t i=0;i<this->map_count();i++) {
        auto region = this->get_map(i);
        size_t base = reinterpret_cast<size_t>(region->baseaddr());
        if (base <= addr && addr < base + region->size()) {
            return region->set_at(addr - base, value);
        }
    }

    throw runtime_error("Address not mapped");
}

bool WinProcess::is_valid_addr(size_t addr) {
    for (size_t i=0;i<this->map_count();i++) {
        auto region = this->get_map(i);
        size_t base = reinterpret_cast<size_t>(region->baseaddr());
        if (base <= addr && addr < base + region->size()) {
            return true;
        }
    }

    return false;
}

MemoryValueRef WinProcess::operator[] (size_t addr) {
    if (!this->is_valid_addr(addr))
        throw runtime_error("Address not mapped");

    return MemoryValueRef(this, addr);
}

